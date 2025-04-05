// SPDX-License-Identifier: MIT

#include <sphinx/screen.hpp>

#include <faroela/api/faroela.hpp>

namespace sphinx {
	[[nodiscard]]
	static inline auto sdl_error(std::source_location location = std::source_location::current()) {
		// TODO: Transform error code.
		return unexpect(SDL_GetError(), error_code::unknown_error, location);
	}

	result<void> initialize_screen_environment() {
		auto success = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_GAMEPAD | SDL_INIT_EVENTS);
		if(!success) [[unlikely]] {
			return sdl_error();
		}

		return {};
	}

	result<void> shutdown_screen_environment() {
		SDL_Quit();

		return {};
	}

	// TODO: Should this be determined by Faroela rather than client?
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
		auto displays = SDL_GetDisplays(&count);
		if(!displays) [[unlikely]] {
			return sdl_error();
		}

		auto primary = SDL_GetPrimaryDisplay();
		if(!primary) [[unlikely]] {
			return sdl_error();
		}

		for(int i = 0; i < count; ++i) {
			auto display = displays[i];

			auto name = SDL_GetDisplayName(display);
			if(!name) [[unlikely]] {
				// TODO: All these error conditions are leaky for displays list.
				return sdl_error();
			}

			std::vector<dimension> resolutions;

			int mode_count;
			auto modes = SDL_GetFullscreenDisplayModes(display, &mode_count);
			if(!modes) [[unlikely]] {
				return sdl_error();
			}

			for(int j = 0; j < mode_count; ++j) {
				auto& mode = modes[j];

				resolutions.emplace_back(dimension{ { static_cast<unsigned>(mode->w), static_cast<unsigned>(mode->h) } });
			}

			info.emplace_back(monitor_info{
					.supported_resolutions = std::move(resolutions),
					.name = std::string(name),
					.primary = primary == display,
					.index = i
			});
		}

		SDL_free(displays);

		return info;
	}

	result<std::unique_ptr<screen>> screen::create(faroela::context* ctx, const graphics_mode& mode) {
		// TODO: Resolve "automatic" API selection here before proceeding.
		// TODO: Do we want to be able to re-create screen to change mode?
		// TODO: Windowed fullscreen and true fullscreen.
		// TODO: HiDPI, HDR and white balance controls.

		auto created = faroela::common::make_unique<screen>(std::nothrow);
		if(!created) [[unlikely]] {
			return unexpect("failed to allocate screen", error_code::out_of_memory);
		}

		SDL_WindowFlags flags = 0;
		if(mode.api == graphics_api::opengl) {
			// TODO: SDL doesn't seem to provide an API for retrieving GLX handles -- should we create with this flag?
			//flags |= SDL_WINDOW_OPENGL;
		}

		created->window = SDL_CreateWindow(std::string(mode.title).data(), static_cast<int>(mode.resolution[0]), static_cast<int>(mode.resolution[1]), flags);
		if(!created->window) [[unlikely]] {
			return sdl_error();
		}

		created->mode = mode;
		created->ctx = ctx;
		created->exit = false;

		auto properties = SDL_GetWindowProperties(created->window);
		if(!properties) [[unlikely]] {
			return sdl_error();
		}

		// TODO: Update clip with resize callback.
		auto success = faroela::api::faroela_render_clip(ctx, 0, 0, mode.resolution[0], mode.resolution[1]);
		if(!success) [[unlikely]] {
			// TODO: Basic result constructs should probably just be imported into the local namespace.
			return faroela::common::unexpect("failed to clip render area");
		}

#ifdef _WIN32
		auto native_window = SDL_GetPointerProperty(properties, SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr);
		auto native_connection = nullptr;
#elif defined(__APPLE__)
		auto native_window = SDL_GetPointerProperty(properties, SDL_PROP_WINDOW_COCOA_WINDOW_POINTER, nullptr);
		auto native_connection = nullptr;
#else
		// TODO: Wayland.
		auto native_window = std::bit_cast<void*>(static_cast<uintptr_t>(SDL_GetNumberProperty(properties, SDL_PROP_WINDOW_X11_WINDOW_NUMBER, 0)));
		auto native_connection = SDL_GetPointerProperty(properties, SDL_PROP_WINDOW_X11_DISPLAY_POINTER, nullptr);
#endif

		success = faroela::api::faroela_render_attach(ctx, native_window, native_connection, nullptr);
		if(!success) [[unlikely]] {
			return faroela::common::unexpect("failed to attach render to screen");
		}

		return created;
	}

	[[nodiscard]]
	result<bool> screen::update() {
		// TODO: What sync rules do we want here for Sphinx.
		auto success = faroela::api::faroela_render_update(ctx, true);
		if(!success) {
			return faroela::common::unexpect("failed update render");
		}

		if(mode.api == graphics_api::opengl) {
			success = SDL_GL_SwapWindow(window);
			if(!success) [[unlikely]] {
				return sdl_error();
			}
		}

		return exit;
	}

	[[nodiscard]]
	result<void> screen::destroy() {
		SDL_DestroyWindow(window);

		return {};
	}

	result<void> screen::register_hid() {
		// TODO: Gamepads.
		faroela::api::faroela_hid_status(ctx, faroela::api::hid_port::desktop, true);

		return {};
	}

	[[nodiscard]]
	result<void> screen::poll() {
		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_EVENT_QUIT: {
					exit = true;
					break;
				}

				case SDL_EVENT_KEY_DOWN: [[fallthrough]];
				case SDL_EVENT_KEY_UP: {
					auto result = handle_keyboard_event(event.key);
					if(!result) [[unlikely]] {
						// TODO: Consume remaining events and hold+log?
						return result;
					}

					break;
				}
			}
		}

		return {};
	}

	result<void> screen::handle_keyboard_event(SDL_KeyboardEvent& event) {
		if(event.repeat) {
			return {};
		}

		// TODO: Scancodes are desirable for anything distributed, but the user-local config may prefer raw so they
		//		 Can rebind to whatever wacky key they may have in local layout.
		auto success = faroela::api::faroela_hid_button_event(ctx, faroela::api::hid_port::desktop, event.scancode, event.down);
		if(!success) [[unlikely]] {
			return unexpect("failed to send keyboard button event", error_code::unknown_error);
		}

		return {};
	}
}
