// SPDX-License-Identifier: MIT

#pragma once

#include <faroela/api/types.hpp>

namespace faroela {
	class hid_state {
	private:
		std::array<bool, faroela::api::hid::button_max> buttons{};
		std::array<float, faroela::api::hid::axis_max> axes{};

	public:
		void set_button(faroela::api::hid::button, bool);
		void set_axis(faroela::api::hid::axis, float);
	};
}
