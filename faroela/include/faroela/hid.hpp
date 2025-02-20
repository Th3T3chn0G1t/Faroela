// SPDX-License-Identifier: MIT

#pragma once

#include <faroela/api/types.hpp>

namespace faroela {
	struct hid_button_event {
		faroela::api::hid::port port;
		faroela::api::hid::button button;
		bool pressed;
	};

	struct hid_axis_event {
		faroela::api::hid::port port;
		faroela::api::hid::axis axis;
		float value;
	};

	struct hid_status_event {
		faroela::api::hid::port port;
		bool connected;
	};

	using hid_event = std::variant<hid_button_event, hid_axis_event, hid_status_event>;

	class hid_state {
	private:
		std::array<bool, faroela::api::hid::button_max> buttons{};
		std::array<float, faroela::api::hid::axis_max> axes{};

	public:
		// TODO: Should hid_state just consume hid_event directly?
		void set_button(faroela::api::hid::button, bool);
		void set_axis(faroela::api::hid::axis, float);
	};
}
