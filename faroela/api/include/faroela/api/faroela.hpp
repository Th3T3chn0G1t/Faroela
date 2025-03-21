// SPDX-License-Identifier: MIT

#pragma once

#ifndef FAROELA_COMMON_EXPORT
# define FAROELA_COMMON_EXPORT
#endif

#include <faroela/api/types.hpp>

// TODO: Would it be wise to have this and a C# equivalent be generated from an API description instead? Would also
//		 Allow for swapping out the intermediate mode of communication (IPC, net etc.).
extern "C" {
	namespace faroela {
		class context;
	}

	namespace faroela::api {
		using link_bool = unsigned;
		using context = faroela::context;

		// NOTE: The client application expects to own the main thread -- so the engine must spawn its own workers in
		//		 here if the system isn't explicitly pumped.
		FAROELA_COMMON_EXPORT link_bool faroela_initialize(context**, int, char**);
		FAROELA_COMMON_EXPORT void faroela_shutdown(context**);

		FAROELA_COMMON_EXPORT void faroela_log(context*, verbosity, const char*);

		// Signal a HID as having been connected or disconnected. Disconnection must refer to a
		// previously-connected port.
		FAROELA_COMMON_EXPORT link_bool faroela_hid_status(context*, hid::port, link_bool);
		FAROELA_COMMON_EXPORT link_bool faroela_hid_button_event(context*, hid::port, hid::button, link_bool);
		FAROELA_COMMON_EXPORT link_bool faroela_hid_axis_event(context*, hid::port, hid::axis, float);
	}
}
