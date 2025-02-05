// SPDX-License-Identifier: MIT

#pragma once

namespace sphinx {
	struct graphics_mode {
		// TODO: Graphic startup options for GLFW vs. what we pass through to
		//		 The engine-proper for BGFX.
	};

	using screen = void*;

	screen make_screen(graphics_mode&);
}
