// SPDX-License-Identifier: MIT

#include <faroela/core.hpp>
#include <faroela/log.hpp>

#include <tracy/Tracy.hpp>

namespace faroela {
	result<void> initialize() {
		faroela::make_default_loggers("faroela.log");

		const auto& logger = spdlog::get("faroela");
		spdlog::stopwatch time;

		logger->info("Initializing...");

		tracy::SetThreadName("faroela_main");

		logger->info("Done. (Took {})", time.elapsed_ms());

		return {};
	}

	void shutdown() {
		const auto& logger = spdlog::get("faroela");
		spdlog::stopwatch time;

		logger->info("Shutting down...");

		logger->info("Done. (Took {})", time.elapsed_ms());

		spdlog::shutdown();
	}
}
