// SPDX-License-Identifier: MIT

#include <faroela/core.hpp>
#include <faroela/log.hpp>

namespace faroela {
	extern "C" FARO_EXPORT void faro_export_initialize(void) {
		make_default_loggers("faroela.log");

		const auto& logger = spdlog::get("faroela");
		spdlog::stopwatch time;

		logger->info("Initializing...");

		logger->info("Done. (Took {})", time.elapsed_ms());
	}

	extern "C" FARO_EXPORT void faro_export_shutdown(void) {
		const auto& logger = spdlog::get("faroela");
		spdlog::stopwatch time;

		logger->info("Shutting down...");

		logger->info("Done. (Took {})", time.elapsed_ms());

		spdlog::shutdown();
	}
}
