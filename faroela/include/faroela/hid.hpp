// SPDX-License-Identifier: MIT

#pragma once

#include <faroela-api/types.hpp>

namespace faroela {
	class hid_state {
	private:
		std::array<bool, faroela_api::hid::button_max> buttons{};
		std::array<float, faroela_api::hid::axis_max> axes{};

	public:
		void set_button(faroela_api::hid::button, bool);
		void set_axis(faroela_api::hid::axis, float);
	};
}
