// SPDX-License-Identifier: MIT

#include <faroela-api/faroela.hpp>

#include <faroela/core.hpp>
#include <faroela/log.hpp>

extern "C" {
	namespace faroela_api {
		FAROELA_EXPORT bool faroela_initialize(void) {
			return faroela::log_result(faroela::initialize());
		}

		FAROELA_EXPORT void faroela_shutdown(void) {
			faroela::shutdown();
		}

		FAROELA_EXPORT void faroela_log(verbosity level, const char* message) {
			faroela::client_log(spdlog::level::level_enum(level), message);
		}
	}
}
