// SPDX-License-Identifier: MIT

#pragma once

#include <faroela/hid.hpp>

#include <faroela/common/result.hpp>

#include <uv.h>

namespace faroela {
	class context {
	public:
		std::shared_ptr<spdlog::logger> logger, api_logger, client_logger;

	private:
		using loop_ptr = std::unique_ptr<uv_loop_t>;
		using loop_ref = std::reference_wrapper<loop_ptr>;

		// TODO: This only expects to hold static strings for now -- but we may need to
		//		 revisit this if we allow the client to register their own event systems.
		unordered_dense::map<std::string_view, loop_ptr> event_systems;

		std::array<std::optional<hid_state>, faroela::api::hid::port_max> hid_states;

	public:
		[[nodiscard]]
		static result<context*> initialize();
		static void shutdown(context*&);

	private:
		[[nodiscard]]
		result<loop_ref> get_system(std::string_view);

		[[nodiscard]]
		result<void> submit(std::string_view, void*);

	public:
		template<typename event_type, typename... args>
		[[nodiscard]]
		result<void> submit(std::string_view system, args&&... v) {
			// TODO: Should event data (both here and underlying submit call) be pulled from a pool rather than new'd?
			event_type* data = new(std::nothrow) event_type(std::forward<args>(v)...);
			if(!data) [[unlikely]] {
				return unexpect("failed to allocate event data", error_code::out_of_memory);
			}

			return forward(submit(system, data));
		}

		[[nodiscard]]
		result<void> add_event_system(std::string_view);

		[[nodiscard]]
		result<void> pump_all();

		[[nodiscard]]
		result<void> pump(std::string_view);
	};
}
