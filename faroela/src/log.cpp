// SPDX-License-Identifier: MIT

#include <faroela/core.hpp>
#include <faroela/log.hpp>

namespace faroela {
	void make_default_loggers(const spdlog::filename_t& file) {
		auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(file, true);
		file_sink->set_level(spdlog::level::trace);

		auto create_named_logger = [&] (std::string_view name) {
			auto console = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
			console->set_level(spdlog::level::trace);
			console->set_pattern(std::format("[{}] [%^%l%$] %v", name));

			auto logger = std::make_shared<spdlog::logger>(std::string(name), spdlog::sinks_init_list{ console, file_sink });
			spdlog::register_logger(logger);
		};

		create_named_logger("faroela");
		create_named_logger("pharaoh");
	}

	extern "C" FARO_EXPORT void faro_export_log(int level, const char* message) {
		spdlog::get("pharaoh")->log(spdlog::level::level_enum(level), std::string_view(message));
	}
}
