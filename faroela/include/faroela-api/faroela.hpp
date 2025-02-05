// SPDX-License-Identifier: MIT

#pragma once

#ifdef _WIN32
# define FAROELA_IMPORT __declspec(dllimport)
#else
# define FAROELA_IMPORT
#endif

extern "C" {
	namespace faroela_api {
		FAROELA_IMPORT bool faroela_initialize(void);
		FAROELA_IMPORT void faroela_shutdown(void);

		enum verbosity : int {
			trace = 0,
			debug = 1,
			info = 2,
			warning = 3,
			error = 4,
			critical = 5
		};

		FAROELA_IMPORT void faroela_log(verbosity, const char*);

		namespace hid {
			enum port : int {
				// Keyboard+mouse are considered a single HID endpoint.
				desktop,

				// Gamepads should be normalized to https://github.com/mdqinc/SDL_GameControllerDB *before* being
				// passed to Faroela.

				gamecon0,
				gamecon1,
				gamecon2,
				gamecon3
			};
		}

		// Signal a HID as having been connected or disconnected. Disconnection must refer to a
		// previously-connected port.
		FAROELA_IMPORT void faroela_hid_status(hid::port, bool);
		FAROELA_IMPORT void faroela_hid_event(hid::port, const char*, bool);
	}
}
