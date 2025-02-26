// SPDX-License-Identifier: MIT

#include <sphinx/screen.hpp>

#include <faroela/api/faroela.hpp>

namespace sphinx {
	static result<void> start(faroela::api::context* ctx) {
		faroela::api::faroela_log(ctx, faroela::api::info, "Hello, Sphinx!");

		auto result = create_screen(graphics_mode{
				.width = 640,
				.height = 480,
				.title = "Sphinx",
				.sync = sync_mode::vsync
		});

		if(!result) [[unlikely]] {
			return forward(result);
		}

		return {};
	}
}

int main(int argc, char** argv) {
	faroela::api::context* ctx;

	if(!faroela::api::faroela_initialize(&ctx, argc, argv)) return 1;

	auto result = sphinx::start(ctx);
	if(result) {
		// TODO: Work out how result passover should work. Make result part of faroela-api?
	}

	faroela::api::faroela_shutdown(&ctx);

	return 0;
}

[[maybe_unused]]
int WinMain(void*, void*, char*, int) {
	return main(__argc, __argv);
}
