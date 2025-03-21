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

		// TODO: Move out HID system initialization.
		result = ctx->add_event_system("hid");
		if(!result) [[unlikely]] {
			return forward(result);
		}

		ctx->hid.status_callback = [&](auto event) {
			ctx->logger->info("HID port '{}' {}", magic_enum::enum_name(event.port), event.connected ? "connected" : "disconnected");
		};

		ctx->hid.button_callback = [&](auto event) {
			ctx->logger->info("HID port '{}' button '{}' {}", magic_enum::enum_name(event.port), magic_enum::enum_name(event.button), event.pressed ? "pressed" : "released");
		};

		ctx->hid.axis_callback = [&](auto event) {
			ctx->logger->info("HID port '{}' axis '{}' at '{}'", magic_enum::enum_name(event.port), magic_enum::enum_name(event.axis), event.value);
		};

		tracy::SetThreadName("faroela_main");

		ctx->logger->info("Done. (Took {})", time.elapsed_ms());

		return ctx;
	}

	void context::shutdown(context*& ctx) {
		spdlog::stopwatch time;

		const auto logger = ctx->logger;
		logger->info("Shutting down...");

		for(auto& system : ctx->event_systems) {
			auto loop = system.second.get();
			unsigned shutdown_tries = 0;

			// See https://stackoverflow.com/questions/25615340/closing-libuv-handles-correctly.
		again:
			uv_walk(loop, [](uv_handle_t* handle, [[maybe_unused]] void* pass) {
				uv_close(handle, handle_close);
			}, nullptr);

			int libuv_result = uv_loop_close(loop);
			if(libuv_result < 0) [[unlikely]] {

				// Loop may need to be flushed before closing -- possibly to run handle close callbacks.
				if(libuv_result == UV_EBUSY) {
					libuv_result = uv_run(loop, UV_RUN_NOWAIT);
					if(libuv_result < 0) {
						log_error(logger, libuv_error(libuv_result));
					}

					// TODO: Configurable?
					if(++shutdown_tries == 3) {
						logger->critical("Could not close event loop '{}' -- skipping...", system.first);
					}

					goto again;
				}

				log_error(logger, libuv_error(libuv_result));
			}
		}

		delete ctx;
		ctx = nullptr;

		logger->info("Done. (Took {})", time.elapsed_ms());

		spdlog::shutdown();
	}

	result<context::loop_ref> context::get_system(std::string_view system) {
		const auto& it = event_systems.find(system);

#ifdef NDEBUG
		if(it == event_systems.end()) [[unlikely]] {
			return unexpect(std::format("no such event system '{}'", system), error_code::key_not_found);
		}
#endif

		return loop_ref(it->second);
	}

	result<void> context::submit(std::string_view system, void* data, async_callback callback) {
		auto result = get_system(system);

		if(!result) {
			return forward(result);
		}

		// TODO: Should event data be pulled from a pool rather than new'd?
		auto async = new(std::nothrow) uv_async_t;
		if(!async) {
			return unexpect("failed to allocate event async", error_code::out_of_memory);
		}

		// TODO: Disable exceptions globally and check all std interfaces. Should we clone with a libc++ we statically
		//		 link so we can disable it from the root?
		int libuv_result = uv_async_init(result->get().get(), async, callback);
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
	result<void> context::add_event_system(std::string_view system) {
		const auto& [ it, emplaced ] = event_systems.try_emplace(system, std::make_unique<uv_loop_t>());

#ifndef NDEBUG
		if(!emplaced) [[unlikely]] {
			return unexpect(std::format("event system '{}' already registered", system), error_code::key_exists);
		}
#endif

		// TODO: Add libuv EH to all calls: https://docs.libuv.org/en/v1.x/errors.html.
		int libuv_result = uv_loop_init(it->second.get());
		if(libuv_result < 0) {
			// TODO: Remove the emplaced member -- we should init the loop first then move it into the map.
			return libuv_error(libuv_result);
		}
		// TODO: IO system will need UV_LOOP_ENABLE_IO_URING_SQPOLL set.

		return {};
	}

	// TODO: Flag for continuing past errors
	result<void> context::pump_all() {
		for(auto& loop : event_systems) {
			int libuv_result = uv_run(loop.second.get(), UV_RUN_NOWAIT);
			if(libuv_result < 0) {
				return libuv_error(libuv_result);
			}
		}

		return {};
	}

	result<void> context::pump(std::string_view system) {
		auto result = get_system(system);

		if(!result) {
			return forward(result);
		}

		int libuv_result = uv_run(result->get().get(), UV_RUN_NOWAIT);
		if(libuv_result < 0) {
			return libuv_error(libuv_result);
		}

		return {};
	}
}
