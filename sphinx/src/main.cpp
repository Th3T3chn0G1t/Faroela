// SPDX-License-Identifier: MIT

#include <sphinx/screen.hpp>

#include <faroela/api/faroela.hpp>

#include <faroela/common/formatters.hpp>

namespace sphinx {
	static result<void> start(faroela::context* ctx) {
		faroela::api::faroela_log(ctx, faroela::api::verbosity::info, "Hello, Sphinx!");

		auto result = initialize_screen_environment();
		if(!result) [[unlikely]] {
			return forward(result);
		}

		auto monitors = get_monitor_info();
		if(!result) [[unlikely]] {
			return forward(result);
		}

		auto primary = std::find_if(monitors->begin(), monitors->end(), [&](auto& monitor) { return monitor.primary; });
		if(primary == monitors->end()) {
			faroela::api::faroela_log(ctx, faroela::api::verbosity::error, "Could not determine primary monitor, assuming index 0");
			primary = monitors->begin();
		}

		auto screen = screen::create(ctx, graphics_mode{
				.resolution = primary->supported_resolutions[0],
				.title = "Sphinx",
				.sync = sync_mode::vsync,
				.api = graphics_api::automatic,
				.monitor_index = static_cast<int>(primary - monitors->begin())
		});

		if(!screen) [[unlikely]] {
			return forward(screen);
		}

		result = (*screen)->register_hid();
		if(!result) [[unlikely]] {
			return forward(result);
		}

		while(true) {
			result = (*screen)->poll_hid();
			if(!result) [[unlikely]] {
				return forward(result);
			}

			auto should_close = (*screen)->update();
			if(!should_close) [[unlikely]] {
				return forward(result);
			}
			else if(should_close.value()) [[unlikely]] {
				break;
			}
		}

		result = (*screen)->destroy();
		if(!result) [[unlikely]] {
			return forward(result);
		}

		result = shutdown_screen_environment();
		if(!result) [[unlikely]] {
			return forward(result);
		}

		return {};
	}
}

int main(int argc, char** argv) {
	faroela::context* ctx;

	auto success = faroela::api::faroela_initialize(&ctx, argc, argv);
	if(!success) {
		return 1;
	}

	auto result = sphinx::start(ctx);
	if(!result) {
		// TODO: Simple log wrapper on Sphinx side. Needs to check error state.
		faroela::api::faroela_log(ctx, faroela::api::verbosity::error, std::format("{}", result).data());
	}

	success = faroela::api::faroela_shutdown(&ctx);
	if(!success) {
		return 1;
	}

	return 0;
}

#ifdef _WIN32
[[maybe_unused]]
int WinMain(void*, void*, char*, int) {
	return main(__argc, __argv);
}
#endif
