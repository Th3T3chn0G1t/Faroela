// SPDX-License-Identifier: MIT

#include <sphinx/screen.hpp>

#include <GLFW/glfw3.h>

namespace sphinx {
	[[nodiscard]]
	static inline result<void> glfw_known_error(std::source_location location = std::source_location::current()) {
		const char* description;
		glfwGetError(&description);

		// The error code check is not neccesary with known error conditions.

		// TODO: Transform error code.
		return unexpect(description, error_code::unknown_error, location);
	}

	[[nodiscard]]
	static inline result<void> glfw_error(std::source_location location = std::source_location::current()) {
		const char* description;
		int code = glfwGetError(&description);

		if(code == GLFW_NO_ERROR) [[likely]] {
			return {};
		}

		// TODO: Transform error code.
		return unexpect(description, error_code::unknown_error, location);
	}

	result<screen> create_screen(const graphics_mode& mode) {
		// TODO: Do we want to be able to re-create screen to change mode?
		if(!glfwInit()) [[unlikely]] {
			return forward(glfw_known_error());
		}

		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		auto result = glfw_error();
		if(!result) [[unlikely]] {
			return forward(result);
		}

		// TODO: Need to specify monitor here for "true" fullscreen. Should we treat windowed fullscreen as a graphics
		//		 mode aswell?
		GLFWwindow* window = glfwCreateWindow(static_cast<int>(mode.resolution[0]), static_cast<int>(mode.resolution[1]), mode.title.data(), nullptr, nullptr);
		if(!window) [[unlikely]] {
			return forward(glfw_known_error());
		}

		sync_mode sync = mode.sync;

		// TODO: Once this is moved out to mode detection -- make these errors non-fatal where possible.
		if(sync == sync_mode::late) {
			int platform = glfwGetPlatform();
			result = glfw_error();
			if(!result) [[unlikely]] {
				return forward(result);
			}

			static constexpr auto platform_late_sync_map = mapbox::eternal::map<int, std::string_view>({
				  { GLFW_PLATFORM_WIN32, "WGL_EXT_swap_control_tear" },
				  { GLFW_PLATFORM_X11, "GLX_EXT_swap_control_tear" }
			});

			bool supported = true;

			const auto it = platform_late_sync_map.find(platform);
			if(it == platform_late_sync_map.end()) {
				supported = false;
			}
			else {
				supported = !!glfwExtensionSupported(it->second.data());
				result = glfw_error();
				if(!result) [[unlikely]] {
					return forward(result);
				}
			}

			if(!supported) {
				sync = sync_mode::vsync;
			}
		}

		static constexpr auto sync_map = mapbox::eternal::map<sync_mode, int>({
				{ sync_mode::none, 0 },
				{ sync_mode::vsync, 1 },
				{ sync_mode::late, -1 }
		});

		glfwSwapInterval(sync_map.at(sync));
		result = glfw_error();
		if(!result) [[unlikely]] {
			return forward(result);
		}

		return static_cast<screen>(window);
	}
}
