// SPDX-License-Identifier: MIT

#pragma once

#include <faroela/api/fwd.hpp>

namespace sphinx {
	enum class sync_mode {
		none,
		vsync,
		// NOTE: Will try and set late sync mode if supported, otherwise fall back to vsync. Graphics mode detection
		//		 API should populate this based on restraints described here: https://www.glfw.org/docs/latest/window_guide.html#buffer_swap.
		// TODO: This fallback behaviour should be disabled once we have graphics mode detection.
		late
	};

	enum class graphics_api {
		automatic,

		opengl,
		vulkan,

		// "Native" here refers to exclusive or non-compatibility APIs for the current platform -- i.e. Metal for Apple platforms
		// And DX for Windows.
		native
	};

	// TODO: Prior stage of graphics mode detection to generate a set of these instead of arbitrary arguments.
	struct graphics_mode {
		// TODO: Graphic startup options for GLFW vs. what we pass through to
		//		 the engine-proper for BGFX.

		unsigned width, height;
		// NOTE: Expect a view of a null-terminated string.
		std::string_view title;

		sync_mode sync;

		graphics_api api;
	};

	using screen = void*;

	// NOTE: Screens are exclusive for Sphinx as we're designing for a homogenous
	//		 Console/Mobile/Desktop client.
	[[nodiscard]]
	result<screen> create_screen(faroela::context*, const graphics_mode&);

	[[nodiscard]]
	result<bool> update_screen(screen);

	[[nodiscard]]
	result<void> destroy_screen(screen);

	[[nodiscard]]
	result<void> register_hid(screen);

	[[nodiscard]]
	result<void> poll_hid(screen);
}
