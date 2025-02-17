// SPDX-License-Identifier: MIT

#pragma once

#include <faroela/hid.hpp>

#include <faroela/common/result.hpp>

namespace faroela {
	class context {
	public:
		std::shared_ptr<spdlog::logger> logger, api_logger, client_logger;

	private:
		std::array<std::optional<hid_state>, faroela::api::hid::port_max> hid_states;

	public:
		static result<context*> initialize();
		static void shutdown(context*&);
	};
}
