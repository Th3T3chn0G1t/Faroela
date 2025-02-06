// SPDX-License-Identifier: MIT

#pragma once

#ifdef _WIN32
# define FAROELA_EXPORT __declspec(dllexport)
#else
# define FAROELA_EXPORT [[gnu::visibility("default")]]
#endif

#include <faroela/result.hpp>

using namespace std::literals::string_view_literals;

namespace faroela {
	struct context {
		std::shared_ptr<spdlog::logger> logger, api_logger, client_logger;
	};

	result<void> initialize(context*&);
	void shutdown(context*&);
}
