// SPDX-License-Identifier: MIT

#include <faroela/core.hpp>

namespace faroela {
	extern "C" FARO_EXPORT void faro_export_initialize(void) {
		auto console = spdlog::stdout_color_mt("console");

		console->info("Initializing Faroela...");

		console->info("Done.");
	}
}
