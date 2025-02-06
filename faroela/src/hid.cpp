// SPDX-License-Identifier: MIT

#include <faroela/hid.hpp>

namespace faroela {
	void hid_state::set_button(faroela_api::hid::button button, bool pressed) {
		buttons[button] = pressed;
	}

	void hid_state::set_axis(faroela_api::hid::axis axis, float value) {
		axes[axis] = value;
	}
}
