// SPDX-License-Identifier: MIT

#pragma once

#include <faroela/hid.hpp>
#include <faroela/delegate.hpp>

#include <faroela/common/result.hpp>

#include <uv.h>

namespace faroela {
	class context {
	public:
		std::shared_ptr<spdlog::logger> logger, api_logger, client_logger;

	private:
		using loop_ptr = std::unique_ptr<uv_loop_t>;

		struct system {
			loop_ptr loop;
			uv_thread_t thread;
		};

		using system_ref = std::reference_wrapper<system>;

		// TODO: This only expects to hold static strings for now -- but we may need to
		//		 revisit this if we allow the client to register their own event systems.
		unordered_dense::map<std::string_view, system> event_systems;

	public:
		hid_system hid;

	public:
		[[nodiscard]]
		static result<context*> initialize();
		static void shutdown(context*&);

		// TODO: Move out to dedicated handle class once we have one.
		static void handle_close(uv_handle_t* handle) {
			// TODO: This is a mess -- we may need to move to C-style allocation functions for handles
			// 		 Because new/delete expect type matches otherwise you get delete size mismatch.
			if(handle->type == UV_IDLE) delete reinterpret_cast<uv_idle_t*>(handle);
			else if(handle->type == UV_ASYNC) delete reinterpret_cast<uv_async_t*>(handle);
			else {
				spdlog::get("faroela")->critical("Cannot delete unexpected handle type '{}'", magic_enum::enum_name(handle->type));
			}
		}

	private:
		[[nodiscard]]
		result<system_ref> get_system(std::string_view);

		static void async_callback_decl(uv_async_t*) noexcept {}
		using async_callback = decltype(async_callback_decl)*;

		[[nodiscard]]
		result<void> submit(std::string_view, void*, async_callback);

	public:
		template<typename event_type>
		[[nodiscard]]
		result<void> submit(std::string_view system, delegate<event_type>* pass) {
			return submit(system, pass, delegate<event_type>::call);
		}

		template<typename event_type, typename... args>
		requires common::is_list_constructible<event_type, args...>
		[[nodiscard]]
		result<void> submit(std::string_view system, delegate<event_type>::callable callable, args&&... v) {
			const auto instance = delegate<event_type>::create(callable, std::forward<args>(v)...);
			if(!instance) [[unlikely]] {
				return forward(instance);
			}

			return submit(system, *instance);
		}

		[[nodiscard]]
		result<void> add_event_system(std::string_view);
	};
}
