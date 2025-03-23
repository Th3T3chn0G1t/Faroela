// SPDX-License-Identifier: MIT

#include <faroela/core.hpp>
#include <faroela/platform.hpp>

#include <faroela/common/log.hpp>

#include <tracy/Tracy.hpp>

namespace faroela {
	[[nodiscard]]
	constexpr inline faroela::common::unexpected libuv_error(int code, std::source_location location = std::source_location::current()) {
		// TODO: Pass through `uv_err_name_r`.
		return unexpect(uv_strerror(code), error_code::unknown_error, location);
	}

	// TODO: Distinguish between context-specific startup/teardown and global
	//		 startup/teardown.
	result<context*> context::initialize() {
		faroela::common::register_default_loggers("faroela.log");

		auto ctx = new(std::nothrow) context;
		if(!ctx) [[unlikely]] {
			return unexpect("failed to allocate context", error_code::out_of_memory);
		}

		ctx->logger = spdlog::get("faroela");
		ctx->api_logger = spdlog::get("faroela-api");
		ctx->client_logger = spdlog::get("client");

		auto result = initialize_platform(*ctx);
		if(!result) [[unlikely]] {
			return forward(result);
		}

		spdlog::stopwatch time;

		ctx->logger->info("Initializing...");

		tracy::SetThreadName("faroela_main");

		ctx->hid.status_callback = [ctx](auto event) {
			ctx->logger->info("HID port '{}' {}", magic_enum::enum_name(event.port), event.connected ? "connected" : "disconnected");
		};

		ctx->hid.button_callback = [ctx](auto event) {
			ctx->logger->info("HID port '{}' button '{}' {}", magic_enum::enum_name(event.port), magic_enum::enum_name(event.button), event.pressed ? "pressed" : "released");
		};

		ctx->hid.axis_callback = [ctx](auto event) {
			ctx->logger->info("HID port '{}' axis '{}' at '{}'", magic_enum::enum_name(event.port), magic_enum::enum_name(event.axis), event.value);
		};

		// TODO: Move out HID system initialization.
		result = ctx->add_event_system("hid");
		if(!result) [[unlikely]] {
			return forward(result);
		}

		ctx->logger->info("Done. (Took {})", time.elapsed_ms());

		return ctx;
	}

	void context::shutdown(context*& ctx) {
		spdlog::stopwatch time;

		const auto logger = ctx->logger;
		logger->info("Shutting down...");

		for(auto& system : ctx->event_systems) {
			auto loop = system.second.loop.get();
			unsigned shutdown_tries = 0;

			// See https://stackoverflow.com/questions/25615340/closing-libuv-handles-correctly.
		again:
			uv_walk(loop, [](uv_handle_t* handle, [[maybe_unused]] void* pass) noexcept {
				uv_close(handle, handle_close);
			}, nullptr);

			uv_stop(loop);

			// TODO: Set up a timer signal to timeout joins here to give up on loop closure.
			int libuv_result = uv_thread_join(&system.second.thread);
			if(libuv_result < 0) [[unlikely]] {
				// TODO: Remove the emplaced member -- we should init the loop first then move it into the map.
				logger->error("{}", libuv_error(libuv_result));
			}

			libuv_result = uv_loop_close(loop);
			if(libuv_result < 0) [[unlikely]] {
				// TODO: Is this necessary?
				if(libuv_result == UV_EBUSY) {
					// TODO: Configurable?
					if(++shutdown_tries == 3) {
						logger->critical("Could not close event loop '{}' -- skipping...", system.first);
					}

					goto again;
				}

				logger->error("{}", libuv_error(libuv_result));
			}
		}

		delete ctx;
		ctx = nullptr;

		logger->info("Done. (Took {})", time.elapsed_ms());

		spdlog::shutdown();
	}

	result<context::system_ref> context::get_system(std::string_view system) {
		const auto& it = event_systems.find(system);

#ifdef NDEBUG
		if(it == event_systems.end()) [[unlikely]] {
			return unexpect(std::format("no such event system '{}'", system), error_code::key_not_found);
		}
#endif

		return system_ref(it->second);
	}

	result<void> context::submit(std::string_view system_name, void* data, async_callback callback) {
		auto result = get_system(system_name);

		if(!result) [[unlikely]] {
			return forward(result);
		}

		// TODO: Should event data be pulled from a pool rather than new'd?
		auto async = new(std::nothrow) uv_async_t;
		if(!async) [[unlikely]] {
			return unexpect("failed to allocate event async", error_code::out_of_memory);
		}

		// TODO: Disable exceptions globally and check all std interfaces. Should we clone with a libc++ we statically
		//		 link so we can disable it from the root?
		int libuv_result = uv_async_init(result->get().loop.get(), async, callback);
		if(libuv_result < 0) [[unlikely]] {
			uv_close(std::bit_cast<uv_handle_t*>(async), handle_close);
			return libuv_error(libuv_result);
		}

		async->data = data;

		libuv_result = uv_async_send(async);
		if(libuv_result < 0) [[unlikely]] {
			// TODO: We probably need a RAII-y wrapper for libuv handles to avoid this kind of mess.
			uv_close(std::bit_cast<uv_handle_t*>(async), handle_close);
			return libuv_error(libuv_result);
		}

		return {};
	}

	// TODO: Flag to mark system as needing to spawn its own thread. Should async systems enter default loop state?
	result<void> context::add_event_system(std::string_view system_name) {
		system system;

		system.loop = common::make_unique<uv_loop_t>(std::nothrow);
		if(!system.loop) [[unlikely]] {
			return unexpect(std::format("failed to allocate event system '{}'", system_name), error_code::out_of_memory);
		}

		auto loop = system.loop.get();

		int libuv_result = uv_loop_init(loop);
		if(libuv_result < 0) [[unlikely]] {
			// TODO: Remove the emplaced member -- we should init the loop first then move it into the map.
			return libuv_error(libuv_result);
		}

		// TODO: IO system will need UV_LOOP_ENABLE_IO_URING_SQPOLL set.
#ifndef NDEBUG
		libuv_result = uv_loop_configure(loop, UV_METRICS_IDLE_TIME);
		if(libuv_result < 0) [[unlikely]] {
			return libuv_error(libuv_result);
		}
#endif

		auto idle_handle = new(std::nothrow) uv_idle_t;
		if(!idle_handle) [[unlikely]] {
			return unexpect(std::format("failed to allocate handle when initializing event system '{}'", system_name), error_code::out_of_memory);
		}

		libuv_result = uv_idle_init(loop, idle_handle);
		if(libuv_result < 0) [[unlikely]] {
			return libuv_error(libuv_result);
		}

		auto idle = [](uv_idle_t*) noexcept {
#ifndef NDEBUG
			// TODO: Metrics from `uv_metrics_idle_time` in here and
			//		 `uv_metrics_info` outside.
#endif
		};

		libuv_result = uv_idle_start(idle_handle, idle);
		if(libuv_result < 0) [[unlikely]] {
			return libuv_error(libuv_result);
		}

		struct thread_create_pass {
			uv_loop_t* loop;
			std::string name;
		}* pass = new(std::nothrow) thread_create_pass{ loop, std::string(system_name) };

		if(!pass) [[unlikely]] {
			return unexpect(std::format("failed to allocate passthrough data when initializing event system '{}'", system_name), error_code::out_of_memory);
		}

		libuv_result = uv_thread_create(&system.thread, [](void* ptr) noexcept {
			auto pass = static_cast<thread_create_pass*>(ptr);

			tracy::SetThreadName(pass->name.data());

			auto loop = pass->loop;

			delete pass;

			int libuv_result = uv_run(loop, UV_RUN_DEFAULT);
			if(libuv_result < 0) [[unlikely]] {
				// TODO: EH?
				return;
			}
		}, pass);

		if(libuv_result < 0) [[unlikely]] {
			return libuv_error(libuv_result);
		}

		const auto& [ it, emplaced ] = event_systems.try_emplace(system_name, std::move(system));

#ifndef NDEBUG
		if(!emplaced) [[unlikely]] {
			// TODO: Free the loop -- a reason to compartmentalise loops (maybe RAII-y?) instead of homogenous [de-]init
			//		 In ctx shutdown.
			return unexpect(std::format("event system '{}' already registered", system_name), error_code::key_exists);
		}
#endif

		return {};
	}
}
