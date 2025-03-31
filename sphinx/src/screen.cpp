// SPDX-License-Identifier: MIT

#include <sphinx/screen.hpp>

#include <faroela/api/faroela.hpp>

// TODO: Sphinx should probably be using SDL instead of GLFW given our portability goals.
//		 This current impl. is mostly just PoC to get stuff working and will be replaced.
#include <GLFW/glfw3.h>
#ifdef _WIN32
// NOTE: This is a bit dodgy but saves us needing to drag in all of Win32 for 2 types.
using HWND = void*;
using HGLRC = void*;
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#elif defined(__APPLE__)
#define GLFW_EXPOSE_NATIVE_COCOA
#define GLFW_EXPOSE_NATIVE_NSGL
#else
// TODO: Wayland runtime switching?
// TODO: Handle EGL & OSMESA.
#define GLFW_EXPOSE_NATIVE_X11
#define GLFW_EXPOSE_NATIVE_GLX
#endif
#define GLFW_NATIVE_INCLUDE_NONE
#include <GLFW/glfw3native.h>

namespace sphinx {
	// The error code check is not necessary with known error conditions.
	[[nodiscard]]
	static inline result<void> glfw_known_error(std::source_location location = std::source_location::current()) {
		const char* description;
		glfwGetError(&description);

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

	static result<screen*> get_window_screen(GLFWwindow* window) {
		auto ptr = glfwGetWindowUserPointer(window);
		auto result = glfw_error();
		if(!result) [[unlikely]] {
			return forward(result);
		}

		return static_cast<screen*>(ptr);
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

	result<void> initialize_screen_environment() {
		if(!glfwInit()) [[unlikely]] {
			return forward(glfw_known_error());
		}

		return {};
	}

	result<void> shutdown_screen_environment() {
		glfwTerminate();
		auto result = glfw_error();
		if(!result) [[unlikely]] {
			return forward(result);
		}

		return {};
	}

	std::span<const graphics_api> get_supported_apis() {
		constexpr auto apis = std::to_array({
				graphics_api::automatic,
				graphics_api::native,

#ifdef __EMSCRIPTEN__
				graphics_api::webgl
#else
# ifndef __cplusplus_winrt
				graphics_api::opengl,
				// TODO: Should we disable Vulkan on Apple platforms [or at least non-macOS Apple platforms]?
				graphics_api::vulkan,
# endif
# if !defined(_WIN32) || (defined(__APPLE__) && !defined(TARGET_OS_MAC))
				graphics_api::opengles,
# endif

# ifdef _WIN32
				graphics_api::directx,
# endif

# ifdef __APPLE__
				graphics_api::metal
# endif
#endif
		});

		return apis;
	}

	[[nodiscard]]
	result<std::vector<monitor_info>> get_monitor_info() {
		std::vector<monitor_info> info;

		int count;
		GLFWmonitor** monitors = glfwGetMonitors(&count);
		auto result = glfw_error();
		if(!result) [[unlikely]] {
			return forward(result);
		}

		GLFWmonitor* primary = glfwGetPrimaryMonitor();
		result = glfw_error();
		if(!result) [[unlikely]] {
			return forward(result);
		}

		for(int i = 0; i < count; ++i) {
			auto monitor = monitors[i];

			const char* name = glfwGetMonitorName(monitor);
			result = glfw_error();
			if(!result) [[unlikely]] {
				return forward(result);
			}

			std::vector<dimension> resolutions;
			int vid_count;
			const GLFWvidmode* modes = glfwGetVideoModes(monitor, &vid_count);
			result = glfw_error();
			if(!result) [[unlikely]] {
				return forward(result);
			}

			for(int j = 0; j < vid_count; ++j) {
				auto& mode = modes[j];

				resolutions.emplace_back(dimension{ { static_cast<unsigned>(mode.width), static_cast<unsigned>(mode.height) } });
			}

			info.emplace_back(monitor_info{
					.supported_resolutions = std::move(resolutions),
					.name = std::string(name),
					.primary = primary == monitor,
					.index = i
			});
		}

		return std::move(info);
	}

	result<std::unique_ptr<screen>> screen::create(faroela::context* ctx, const graphics_mode& mode) {
		// TODO: Resolve "automatic" API selection here before proceeding.
		// TODO: Do we want to be able to re-create screen to change mode?
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
		GLFWwindow* window = glfwCreateWindow(static_cast<int>(mode.resolution[0]), static_cast<int>(mode.resolution[1]), std::string(mode.title).data(), nullptr, nullptr);
		if(!window) [[unlikely]] {
			return forward(glfw_known_error());
		}

		auto retval = faroela::common::make_unique<screen>(std::nothrow);
		if(!retval) [[unlikely]] {
			// TODO: Clean up dead init state.
			return unexpect("failed to allocate screen", error_code::out_of_memory);
		}

		retval->mode = mode;
		retval->ctx = ctx;
		retval->handle = window;

		glfwSetWindowUserPointer(window, retval.get());
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

		// NOTE: Intentionally no EH on these GLFW calls for simplicity and because bgfx handles nullptr gracefully
		//		 On the Faroela side.
#ifdef _WIN32
		auto native_window = glfwGetWin32Window;
		auto native_connection = [](GLFWwindow*) { return nullptr; };
		auto native_context = glfwGetWGLContext;
#elif defined(__APPLE__)
		auto native_window = glfwGetCocoaWindow;
		auto native_connection = [](GLFWwindow*) { return nullptr; };
		auto native_context = glfwGetNSGLContext;
#else
		auto native_window = glfwGetX11Window;
		auto native_connection = glfwGetX11Display;
		auto native_context = glfwGetGLXContext;
#endif

		// TODO: Update clip with resize callback.
		auto success = faroela::api::faroela_render_clip(ctx, 0, 0, mode.resolution[0], mode.resolution[1]);
		if(!success) {
			// TODO: Basic result constructs should probably just be imported into the local namespace.
			return faroela::common::unexpect("failed to clip render area");
		}

		success = faroela::api::faroela_render_attach(ctx, native_window(window), native_connection(window), native_context(window));
		// Clear error state.
		glfwGetError(nullptr);
		if(!success) {
			return faroela::common::unexpect("failed to attach render to screen");
		}

		return retval;
	}

	[[nodiscard]]
	result<bool> screen::update() {
		auto window = static_cast<GLFWwindow*>(handle);

		bool should_close = !!glfwWindowShouldClose(window);
		auto result = glfw_error();
		if(!result) [[unlikely]] {
			return forward(result);
		}

		// TODO: What sync rules do we want here for Sphinx.
		auto success = faroela::api::faroela_render_update(ctx, true);
		if(!success) {
			return faroela::common::unexpect("failed to attach render to screen");
		}

		if(mode.api == graphics_api::opengl) {
			glfwSwapBuffers(window);
			result = glfw_error();
			if(!result) [[unlikely]] {
				return forward(result);
			}
		}

		return should_close;
	}

	[[nodiscard]]
	result<void> screen::destroy() {
		auto window = static_cast<GLFWwindow*>(handle);

		glfwDestroyWindow(window);
		auto result = glfw_error();
		if(!result) [[unlikely]] {
			return forward(result);
		}

		return {};
	}

	result<void> screen::register_hid() {
		auto window = static_cast<GLFWwindow*>(handle);

		faroela::api::faroela_hid_status(ctx, faroela::api::hid_port::desktop, true);

		glfwSetKeyCallback(window, [](GLFWwindow* window, int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mods) noexcept {
			if(key < 0) {
				// Ignore unknown keys.
				return;
			}

			// TODO: EH here?
			auto screen = get_window_screen(window);

			auto button = static_cast<faroela::api::hid_button>(key);
			faroela::api::faroela_hid_button_event((*screen)->ctx, faroela::api::hid_port::desktop, button, action == GLFW_PRESS);
		});
		auto result = glfw_error();
		if(!result) [[unlikely]] {
			return forward(result);
		}

		return {};
	}

	[[nodiscard]]
	result<void> screen::poll_hid() {
		glfwPollEvents();
		auto result = glfw_error();
		if(!result) [[unlikely]] {
			return forward(result);
		}

		return {};
	}
}
