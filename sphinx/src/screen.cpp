// SPDX-License-Identifier: MIT

#include <sphinx/screen.hpp>

#include <faroela/api/faroela.hpp>

#include <GLFW/glfw3.h>

namespace sphinx {
	struct window_userdata {
		faroela::context* ctx;
		graphics_mode mode;
	};

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

	static result<window_userdata*> get_window_userdata(GLFWwindow* window) {
		auto ptr = glfwGetWindowUserPointer(window);
		auto result = glfw_error();
		if(!result) [[unlikely]] {
			return forward(result);
		}

		auto ctx = static_cast<window_userdata*>(ptr);

		return ctx;
	}

	static result<void> set_gl_state(GLFWwindow* window, const graphics_mode& mode) {
		glfwMakeContextCurrent(window);
		auto result = glfw_error();
		if(!result) [[unlikely]] {
			return forward(result);
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

			bool supported;

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

		return {};
	}

	result<screen> create_screen(faroela::context* ctx, const graphics_mode& mode) {
		// TODO: Resolve "automatic" API selection here before proceeding.

		// TODO: Do we want to be able to re-create screen to change mode?
		if(!glfwInit()) [[unlikely]] {
			return forward(glfw_known_error());
		}

		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		auto result = glfw_error();
		if(!result) [[unlikely]] {
			return forward(result);
		}

		glfwWindowHint(GLFW_CLIENT_API, mode.api == graphics_api::opengl ? GLFW_OPENGL_API : GLFW_NO_API);
		result = glfw_error();
		if(!result) [[unlikely]] {
			return forward(result);
		}

		// TODO: Need to specify monitor here for "true" fullscreen. Should we treat windowed fullscreen as a graphics
		//		 mode aswell?
		GLFWwindow* window = glfwCreateWindow(static_cast<int>(mode.resolution[0]), static_cast<int>(mode.resolution[1]), mode.title.data(), nullptr, nullptr);
		if(!window) [[unlikely]] {
			return forward(glfw_known_error());
		}

		auto userdata = new(std::nothrow) window_userdata{ ctx, mode };
		glfwSetWindowUserPointer(window, userdata);
		result = glfw_error();
		if(!result) [[unlikely]] {
			return forward(result);
		}

		if(mode.api == graphics_api::opengl) {
			result = set_gl_state(window, mode);
			if(!result) [[unlikely]] {
				return forward(result);
			}
		}

		return static_cast<screen>(window);
	}

	[[nodiscard]]
	result<bool> update_screen(screen screen) {
		auto window = static_cast<GLFWwindow*>(screen);
		auto userdata = get_window_userdata(window);
		if(!userdata) [[unlikely]] {
			return forward(userdata);
		}

		if(userdata.value()->mode.api == graphics_api::opengl) {
			glfwSwapBuffers(window);
			auto result = glfw_error();
			if(!result) [[unlikely]] {
				return forward(result);
			}
		}

		bool should_close = glfwWindowShouldClose(window);
		auto result = glfw_error();
		if(!result) [[unlikely]] {
			return forward(result);
		}

		return should_close;
	}

	[[nodiscard]]
	result<void> destroy_screen(screen screen) {
		auto window = static_cast<GLFWwindow*>(screen);

		glfwDestroyWindow(window);
		auto result = glfw_error();
		if(!result) [[unlikely]] {
			return forward(result);
		}

		return {};
	}

	result<void> register_hid(screen screen) {
		auto window = static_cast<GLFWwindow*>(screen);
		auto userdata = get_window_userdata(window);
		if(!userdata) [[unlikely]] {
			return forward(userdata);
		}

		faroela::api::faroela_hid_status(userdata.value()->ctx, faroela::api::hid::port::desktop, true);

		glfwSetKeyCallback(window, [](GLFWwindow* window, int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mods) {
			// TODO: EH here?
			auto userdata = get_window_userdata(window);

			auto button = static_cast<faroela::api::hid::button>(key);
			faroela::api::faroela_hid_button_event(userdata.value()->ctx, faroela::api::hid::port::desktop, button, action == GLFW_PRESS);
		});
		auto result = glfw_error();
		if(!result) [[unlikely]] {
			return forward(result);
		}

		return {};
	}

	[[nodiscard]]
	result<void> poll_hid(screen screen) {
		auto window = static_cast<GLFWwindow*>(screen);
		auto userdata = get_window_userdata(window);
		if(!userdata) [[unlikely]] {
			return forward(userdata);
		}

		glfwPollEvents();
		auto result = glfw_error();
		if(!result) [[unlikely]] {
			return forward(result);
		}

		return {};
	}
}
