// SPDX-License-Identifier: MIT

#include <faroela/core.hpp>
#include <faroela/log.hpp>

#include <tracy/Tracy.hpp>

namespace faroela {
	// TODO: Distinguish between context-specific startup/teardown and global
	//		 startup/teardown.
	result<void> initialize(context*& ctx) {
		faroela::make_default_loggers("faroela.log");

		if(!(ctx = new(std::nothrow) context)) {
			return unexpected("failed to allocate context", error_code::out_of_memory);
		}

		ctx->logger = spdlog::get("faroela");
		ctx->api_logger = spdlog::get("faroela-api");
		ctx->client_logger = spdlog::get("client");

		spdlog::stopwatch time;

		ctx->logger->info("Initializing...");

		tracy::SetThreadName("faroela_main");

		ctx->logger->info("Done. (Took {})", time.elapsed_ms());

		return {};
	}

	void shutdown(context*& ctx) {
		spdlog::stopwatch time;

		const auto logger = ctx->logger;
		logger->info("Shutting down...");

		delete ctx;
		ctx = nullptr;

		logger->info("Done. (Took {})", time.elapsed_ms());

		spdlog::shutdown();
	}
}
