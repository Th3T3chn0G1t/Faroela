// SPDX-License-Identifier: MIT

#pragma once

#include <faroela/delegate.hpp>

#include <faroela/hid.hpp>
#include <faroela/render.hpp>
#include <faroela/vfs.hpp>

#include <faroela/common/result.hpp>

namespace faroela {
	[[nodiscard]]
	inline faroela::common::unexpected libuv_error(int code, std::source_location location = std::source_location::current()) {
		// TODO: Pass through `uv_err_name_r`.
		return unexpect(uv_strerror(code), error_code::unknown_error, location);
	}

	class context {
	public:
		std::shared_ptr<spdlog::logger> logger, api_logger, client_logger;

	private:
		struct system {
			uv_loop_t loop;
			uv_thread_t thread;
			uv_idle_t idle;
		};

		using system_ref = std::reference_wrapper<system>;

		// TODO: This only expects to hold static strings for now -- but we may need to
		//		 revisit this if we allow the client to register their own event systems.
		unordered_dense::map<std::string_view, std::unique_ptr<system>> event_systems;

	public:
		hid_system hid;
		vfs_system vfs;
		render_system render;

	private:
		context() = default;

	public:
		// TODO: These should be changed to the standard `create`+destructor instead of this.
		[[nodiscard]]
		static result<context*> initialize();
		static void shutdown(context*&);

	public:
		context(const context&) = delete; // copy constructor
		context& operator=(const context&) = delete; // copy assignment

		context(context&&) = delete; // move constructor
		context& operator=(context&&) noexcept = delete; // move assignment

	private:
		// NOTE: Need this workaround to have `noexcept` on `uv_async_cb` params.
		static void async_callback_decl(uv_handle_t*) noexcept {}
		using async_callback = decltype(async_callback_decl)*;

		[[nodiscard]]
		result<void> submit(std::string_view, void*, async_callback);

	public:
		[[nodiscard]]
		result<system_ref> get_system(std::string_view);

		template<typename delegate_type>
		[[nodiscard]]
		result<void> submit(std::string_view system, delegate_type* pass) {
			return submit(system, pass, reinterpret_cast<async_callback>(delegate_type::call));
		}

		template<typename delegate_type, typename... args>
		requires(common::is_list_constructible<typename delegate_type::event, args...>)
		[[nodiscard]]
		result<void> submit(std::string_view system, delegate_type::callable callable, args&&... v) {
			const auto instance = delegate_type::create(callable, std::forward<args>(v)...);
			if(!instance) [[unlikely]] {
				return forward(instance);
			}

			return submit(system, *instance);
		}

		[[nodiscard]]
		result<void> add_system(std::string_view);

		// TODO: Add tracking system for removing idlers during runtime.
		[[nodiscard]]
		result<uv_idle_t*> add_idler(std::string_view, idle_delegate*);

		[[nodiscard]]
		result<void> remove_idler(uv_idle_t*);
	};
}
