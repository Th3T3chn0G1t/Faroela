// SPDX-License-Identifier: MIT

#include <faroela/hid.hpp>

namespace faroela {
	void hid_state::set_button(faroela::api::hid_button button, bool pressed) {
		buttons[button] = pressed;
	}

	void hid_state::set_axis(faroela::api::hid_axis axis, float value) {
		axes[axis] = value;
	}
}
