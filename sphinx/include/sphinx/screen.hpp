// SPDX-License-Identifier: MIT

#pragma once

namespace faroela {
	class context;
}

namespace sphinx {
	using dimension = std::array<unsigned, 2>;

	enum class sync_mode {
		none,
		vsync,
		// NOTE: Will try and set late sync mode if supported with GL API, otherwise fall back to vsync.
		late
	};

	enum class graphics_api {
		automatic,

		opengl,
		opengles,

		vulkan,

		// Latest supported DX.
		directx,

		metal,

		webgl,

		// "Native" here refers to exclusive or non-compatibility APIs for the current platform --
		// i.e. Metal for Apple platforms and the latest supported DX for Windows.
		native
	};

	// TODO: This doesn't enumerate video modes as resolution+refresh combos, and supported refresh rates are currently
	//		 Decoupled from screen mode selection.
	struct monitor_info {
		std::vector<dimension> supported_resolutions;
		std::string name;
		bool primary;
		int index;
	};

	struct graphics_mode {
		// TODO: Graphic startup options for GLFW vs. what we pass through to
		//		 the engine-proper for BGFX.

		dimension resolution;
		std::string_view title;

		sync_mode sync;

		graphics_api api;

		// NOTE: Set to -1 for primary monitor.
		int monitor_index;
	};

	// NOTE: Must be called before the initialization of a screen or retrieval
	//		 Of monitor information.
	[[nodiscard]]
	result<void> initialize_screen_environment();

	[[nodiscard]]
	result<void> shutdown_screen_environment();

	[[nodiscard]]
	std::span<const graphics_api> get_supported_apis();

	// TODO: This data can be invalidated by monitor configuration changes -- should return a reference to shared state
	//		 Which unfortunately must have a global components as the monitor callbacks do not provide a mechanism for
	//		 Any userdata retrieval.
	[[nodiscard]]
	result<std::vector<monitor_info>> get_monitor_info();

	// NOTE: Screens are nominally exclusive for Sphinx as we're designing for a homogenous
	//		 Console/Mobile/Desktop client.
	class screen {
	public:
		graphics_mode mode;

	private:
		faroela::context* ctx;
		void* handle;

	public:
		[[nodiscard]]
		static result<std::unique_ptr<screen>> create(faroela::context*, const graphics_mode&);

	public:
		// TODO: Why does this have an explicit destroy function instead of a destructor?
		[[nodiscard]]
		result<void> destroy();

		[[nodiscard]]
		result<bool> update();

		// NOTE: Registers HID state with Faroela.
		[[nodiscard]]
		result<void> register_hid();

		[[nodiscard]]
		result<void> poll_hid();
	};
}
