// SPDX-License-Identifier: MIT

#include <faroela/core.hpp>
#include <faroela/log.hpp>

namespace faroela {
	extern "C" FARO_EXPORT void faro_export_initialize(void) {
		make_default_loggers("faroela.log");

		const auto& logger = spdlog::get("faroela");

		logger->info("Initializing Faroela...");

		logger->info("Done.");
	}

	extern "C" FARO_EXPORT void faro_export_shutdown(void) {
		spdlog::shutdown();
	}
}
