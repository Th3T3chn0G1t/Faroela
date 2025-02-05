// SPDX-License-Identifier: MIT

#include <faroela-api/faroela.hpp>

#include <faroela/core.hpp>
#include <faroela/log.hpp>

extern "C" {
	namespace faroela_api {
		FARO_EXPORT bool faro_initialize(void) {
			return faroela::log_result(faroela::initialize());
		}

		FARO_EXPORT void faro_shutdown(void) {
			faroela::shutdown();
		}

		FARO_EXPORT void faro_log(verbosity level, const char* message) {
			faroela::client_log(spdlog::level::level_enum(level), message);
		}
	}
}
