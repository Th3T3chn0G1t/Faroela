// SPDX-License-Identifier: MIT

#pragma once

#include <faroela/delegate.hpp>

#include <faroela/api/types.hpp>

namespace faroela {
	struct hid_status_event {
		faroela::api::hid::port port;
		bool connected;
	};

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

	class hid_state {
	private:
		std::array<bool, faroela::api::hid::button_max> buttons{};
		std::array<float, faroela::api::hid::axis_max> axes{};

	public:
		void set_button(faroela::api::hid::button, bool);
		void set_axis(faroela::api::hid::axis, float);
	};

	class hid_system {
	public:
		std::array<std::optional<hid_state>, faroela::api::hid::port_max> hid_states;

		// TODO: Since this basically just acts as a store -- these don't need to be std::function; just a temporary fix.
		std::function<void(hid_status_event&)> status_callback;
		std::function<void(hid_button_event&)> button_callback;
		std::function<void(hid_axis_event&)> axis_callback;
	};
}
