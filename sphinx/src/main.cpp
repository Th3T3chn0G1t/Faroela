// SPDX-License-Identifier: MIT

#include <sphinx/screen.hpp>

#include <faroela/api/faroela.hpp>

#include <faroela/common/formatters.hpp>

namespace sphinx {
	static result<void> start(faroela::context* ctx) {
		faroela::api::faroela_log(ctx, faroela::api::info, "Hello, Sphinx!");

		auto screen = create_screen(ctx, graphics_mode{
				.resolution = { 640, 480 },
				.title = "Sphinx",
				.sync = sync_mode::vsync,
				.api = graphics_api::automatic
		});

		if(!screen) [[unlikely]] {
			return forward(screen);
		}

		auto result = register_hid(screen.value());
		if(!result) [[unlikely]] {
			return forward(result);
		}

		while(true) {
			result = poll_hid(screen.value());
			if(!result) [[unlikely]] {
				return forward(result);
			}

			auto should_close = update_screen(screen.value());
			if(!should_close) [[unlikely]] {
				return forward(result);
			}
			else if(should_close.value()) [[unlikely]] {
				break;
			}
		}

		result = destroy_screen(screen.value());
		if(!result) [[unlikely]] {
			return forward(result);
		}

		return {};
	}
}

int main(int argc, char** argv) {
	faroela::context* ctx;

	if(!faroela::api::faroela_initialize(&ctx, argc, argv)) return 1;

	auto result = sphinx::start(ctx);
	if(!result) {
		// TODO: Simple log wrapper on Sphinx side.
		faroela::api::faroela_log(ctx, faroela::api::error, std::format("{}", result).data());
	}

	faroela::api::faroela_shutdown(&ctx);

	return 0;
}

#ifdef _WIN32
[[maybe_unused]]
int WinMain(void*, void*, char*, int) {
	return main(__argc, __argv);
}
#endif
