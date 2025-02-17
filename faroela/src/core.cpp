// SPDX-License-Identifier: MIT

#include <faroela/core.hpp>
#include <faroela/platform.hpp>

#include <faroela/common/log.hpp>

#include <tracy/Tracy.hpp>

namespace faroela {
	// TODO: Distinguish between context-specific startup/teardown and global
	//		 startup/teardown.
	result<context*> context::initialize() {
		context* ctx;

		faroela::common::register_default_loggers("faroela.log");

		if(!(ctx = new(std::nothrow) context)) {
			return unexpect("failed to allocate context", error_code::out_of_memory);
		}

		ctx->logger = spdlog::get("faroela");
		ctx->api_logger = spdlog::get("faroela-api");
		ctx->client_logger = spdlog::get("client");

		auto result = initialize_platform(*ctx);
		if(!result) return forward(result);

		spdlog::stopwatch time;

		ctx->logger->info("Initializing...");

		tracy::SetThreadName("faroela_main");

		ctx->logger->info("Done. (Took {})", time.elapsed_ms());

		return ctx;
	}

	void context::shutdown(context*& ctx) {
		spdlog::stopwatch time;

		const auto logger = ctx->logger;
		logger->info("Shutting down...");

		delete ctx;
		ctx = nullptr;

		logger->info("Done. (Took {})", time.elapsed_ms());

		spdlog::shutdown();
	}
}
