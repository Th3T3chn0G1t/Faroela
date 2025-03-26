// SPDX-License-Identifier: MIT

#include <faroela/common/log.hpp>
#include <faroela/common/formatters.hpp>

namespace faroela::common {
	using common::logger;

	// TODO: `make_shared` should have nothrow wrapper like `make_unique`.
	void register_default_loggers(const spdlog::filename_t& file) {
		auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(file, true);
		file_sink->set_level(spdlog::level::trace);

#ifdef _WIN32
		auto debug_sink = std::make_shared<spdlog::sinks::msvc_sink<std::mutex>>(true);
#endif

		auto create_named_logger = [&](std::string_view name) {
			auto console = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
			console->set_level(spdlog::level::trace);
			console->set_pattern(std::format("[{}] [%^%l%$] %v", name));

			auto sinks = spdlog::sinks_init_list{
					console,
#ifdef _WIN32
					debug_sink,
#endif
					file_sink
			};
			auto logger = std::make_shared<spdlog::logger>(std::string(name), std::move(sinks));
			spdlog::register_logger(logger);
		};

		create_named_logger("faroela");
		create_named_logger("faroela-api");
		create_named_logger("client");
	}
}
