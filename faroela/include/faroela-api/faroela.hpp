// SPDX-License-Identifier: MIT

#pragma once

#ifndef FAROELA_EXPORT
# define FAROELA_EXPORT
#endif

// TODO: Would it be wise to have this and a C# equivalent be generated from an API description instead? Would also
//		 Allow for swapping out the intermediate mode of communication (IPC, net etc.).
extern "C" {
	namespace faroela {
		struct context;
	}

	namespace faroela_api {
		using context = faroela::context;

		FAROELA_EXPORT bool faroela_initialize(context**);
		FAROELA_EXPORT void faroela_shutdown(context**);

		enum verbosity : int {
			trace = 0,
			debug = 1,
			info = 2,
			warning = 3,
			error = 4,
			critical = 5
		};

		FAROELA_EXPORT void faroela_log(context*, verbosity, const char*);

		namespace hid {
			enum port : int {
				// Keyboard+mouse are considered a single HID endpoint.
				desktop = 0,

				// Gamepads should be normalized to https://github.com/mdqinc/SDL_GameControllerDB *before* being
				// passed to Faroela.

				gamecon0 = 1,
				gamecon1 = 2,
				gamecon2 = 3,
				gamecon3 = 4
			};

			// Desktop and gamecon ranges overlap.
			enum button : int {
				mouse_right = 0,
				mouse_middle = 1,
				mouse_left = 2,

				// Keynames subset maps directly to GLFW for convenience.
				key_space = 32,
				key_apostrophe = 39,
				key_comma = 44,
				key_minus = 45,
				key_period = 46,
				key_slash = 47,
				key_0 = 48,
				key_1 = 49,
				key_2 = 50,
				key_3 = 51,
				key_4 = 52,
				key_5 = 53,
				key_6 = 54,
				key_7 = 55,
				key_8 = 56,
				key_9 = 57,
				key_semicolon = 59,
				key_equal = 61,
				key_a = 65,
				key_b = 66,
				key_c = 67,
				key_d = 68,
				key_e = 69,
				key_f = 70,
				key_g = 71,
				key_h = 72,
				key_i = 73,
				key_j = 74,
				key_k = 75,
				key_l = 76,
				key_m = 77,
				key_n = 78,
				key_o = 79,
				key_p = 80,
				key_q = 81,
				key_r = 82,
				key_s = 83,
				key_t = 84,
				key_u = 85,
				key_v = 86,
				key_w = 87,
				key_x = 88,
				key_y = 89,
				key_z = 90,
				key_left_bracket = 91,
				key_backslash = 92,
				key_right_bracket = 93,
				key_grave_accent = 96,
				key_escape = 256,
				key_enter = 257,
				key_tab = 258,
				key_backspace = 259,
				key_insert = 260,
				key_delete = 261,
				key_right = 262,
				key_left = 263,
				key_down = 264,
				key_up = 265,
				key_page_up = 266,
				key_page_down = 267,
				key_home = 268,
				key_end = 269,
				key_caps_lock = 280,
				key_scroll_lock = 281,
				key_num_lock = 282,
				key_print_screen = 283,
				key_pause = 284,
				key_f1 = 290,
				key_f2 = 291,
				key_f3 = 292,
				key_f4 = 293,
				key_f5 = 294,
				key_f6 = 295,
				key_f7 = 296,
				key_f8 = 297,
				key_f9 = 298,
				key_f10 = 299,
				key_f11 = 300,
				key_f12 = 301,
				key_f13 = 302,
				key_f14 = 303,
				key_f15 = 304,
				key_f16 = 305,
				key_f17 = 306,
				key_f18 = 307,
				key_f19 = 308,
				key_f20 = 309,
				key_f21 = 310,
				key_f22 = 311,
				key_f23 = 312,
				key_f24 = 313,
				key_f25 = 314,
				key_keypad_0 = 320,
				key_keypad_1 = 321,
				key_keypad_2 = 322,
				key_keypad_3 = 323,
				key_keypad_4 = 324,
				key_keypad_5 = 325,
				key_keypad_6 = 326,
				key_keypad_7 = 327,
				key_keypad_8 = 328,
				key_keypad_9 = 329,
				key_keypad_decimal = 330,
				key_keypad_divide = 331,
				key_keypad_multiply = 332,
				key_keypad_subtract = 333,
				key_keypad_add = 334,
				key_keypad_enter = 335,
				key_keypad_equal = 336,
				key_left_shift = 340,
				key_left_control = 341,
				key_left_alt = 342,
				key_left_super = 343,
				key_right_shift = 344,
				key_right_control = 345,
				key_right_alt = 346,
				key_right_super = 347,
				key_menu = 348,

				// Gamepad constants map directly to GLFW for convenience.
				gamecon_a = 0,
				gamecon_b = 1,
				gamecon_x = 2,
				gamecon_y = 3,
				gamecon_left_bumper = 4,
				gamecon_right_bumper = 5,
				gamecon_back = 6,
				gamecon_start = 7,
				gamecon_guide = 8,
				gamecon_left_thumb = 9,
				gamecon_right_thumb = 10,
				gamecon_dpad_up = 11,
				gamecon_dpad_right = 12,
				gamecon_dpad_down = 13,
				gamecon_dpad_left = 14,
			};

			enum axis : int {
				mouse_x = 0,
				mouse_y = 1,

				// Gamepad constants map directly to GLFW for convenience.
				gamecon_left_stick_x = 0,
				gamecon_left_stick_y = 1,
				gamecon_right_stick_x = 2,
				gamecon_right_stick_y = 3,
				gamecon_left_trigger = 4,
				gamecon_right_trigger = 5
			};
		}

		// Signal a HID as having been connected or disconnected. Disconnection must refer to a
		// previously-connected port.
		FAROELA_EXPORT void faroela_hid_status(context*, hid::port, bool);
		FAROELA_EXPORT void faroela_hid_button_event(context*, hid::port, hid::button, bool);
		FAROELA_EXPORT void faroela_hid_axis_event(context*, hid::port, hid::axis, float);
	}
}
