// SPDX-License-Identifier: MIT

#pragma once

namespace sphinx {
	enum class sync_mode {
		none,
		vsync,
		// NOTE: Will try and set late sync mode if supported, otherwise fall back to vsync. Graphics mode detection
		//		 API should populate this based on restraints described here: https://www.glfw.org/docs/latest/window_guide.html#buffer_swap.
		// TODO: This fallback behaviour should be disabled once we have graphics mode detection.
		late
	};

	// TODO: Prior stage of graphics mode detection to generate a set of these instead of arbitrary arguments.
	struct graphics_mode {
		// TODO: Graphic startup options for GLFW vs. what we pass through to
		//		 the engine-proper for BGFX.

		unsigned width, height;
		// NOTE: Expect a view of a null-terminated string.
		std::string_view title;

		sync_mode sync;
	};

	using screen = void*;

	// NOTE: Screens are exclusive for Sphinx as we're designing for a homogenous
	//		 Console/Mobile/Desktop client.
	[[nodiscard]]
	result<screen> create_screen(const graphics_mode&);
}
