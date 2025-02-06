// SPDX-License-Identifier: MIT

#pragma once

#include <faroela/result.hpp>
#include <faroela/hid.hpp>

namespace faroela {
	class context {
	public:
		std::shared_ptr<spdlog::logger> logger, api_logger, client_logger;

	private:
		std::array<std::optional<hid_state>, faroela_api::hid::port_max> hid_states;

	public:
		static result<context*> initialize();
		static void shutdown(context*&);
	};
}
