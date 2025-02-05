// SPDX-License-Identifier: MIT

#pragma once

#ifdef _WIN32
# define FARO_IMPORT __declspec(dllimport)
#else
# define FARO_IMPORT
#endif

extern "C" {
	namespace faroela_api {
		FARO_IMPORT bool faro_initialize(void);
		FARO_IMPORT void faro_shutdown(void);

		enum verbosity : int {
			trace = 0,
			debug = 1,
			info = 2,
			warning = 3,
			error = 4,
			critical = 5
		};

		FARO_IMPORT void faro_log(verbosity, const char*);

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
		FARO_IMPORT void faro_hid_status(hid::port, bool);
		FARO_IMPORT void faro_hid_event(hid::port, const char*, bool);
	}
}
