// SPDX-License-Identifier: MIT

#pragma once

#include <faroela/delegate.hpp>

#include <faroela/hid.hpp>
#include <faroela/render.hpp>

#include <faroela/common/result.hpp>

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
		render_system render;

	public:
		// TODO: These should be changed to the standard `create`+destructor instead of this.
		[[nodiscard]]
		static result<context*> initialize();
		static void shutdown(context*&);

	private:
		[[nodiscard]]
		result<system_ref> get_system(std::string_view);

		// NOTE: Need this workaround to have `noexcept` on `uv_async_cb` params.
		static void async_callback_decl(uv_handle_t*) noexcept {}
		using async_callback = decltype(async_callback_decl)*;

		[[nodiscard]]
		result<void> submit(std::string_view, void*, async_callback);

	public:
		template<typename event_type>
		[[nodiscard]]
		result<void> submit(std::string_view system, delegate<event_type, true>* pass) {
			return submit(system, pass, delegate<event_type, true>::call);
		}

		template<typename event_type, typename... args>
		requires(common::is_list_constructible<event_type, args...>)
		[[nodiscard]]
		result<void> submit(std::string_view system, delegate<event_type, true>::callable callable, args&&... v) {
			const auto instance = delegate<event_type, true>::create(callable, std::forward<args>(v)...);
			if(!instance) [[unlikely]] {
				return forward(instance);
			}

			return submit(system, *instance);
		}

		[[nodiscard]]
		result<void> add_system(std::string_view);

		// TODO: Add tracking system for removing idlers during runtime.
		[[nodiscard]]
		result<void> add_idler(std::string_view, delegate<delegate_dummy, false>*);
	};
}
