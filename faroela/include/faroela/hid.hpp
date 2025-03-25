// SPDX-License-Identifier: MIT

#pragma once

#include <faroela/delegate.hpp>

#include <faroela/api/faroela.hpp>

namespace faroela {
	struct hid_status_event {
		faroela::api::hid_port port;
		bool connected;
	};

	struct hid_button_event {
		faroela::api::hid_port port;
		faroela::api::hid_button button;
		bool pressed;
	};

	struct hid_axis_event {
		faroela::api::hid_port port;
		faroela::api::hid_axis axis;
		float value;
	};

	class hid_state {
	private:
		std::array<bool, faroela::api::button_max> buttons{};
		std::array<float, faroela::api::axis_max> axes{};

	public:
		void set_button(faroela::api::hid_button, bool);
		void set_axis(faroela::api::hid_axis, float);
	};

	class hid_system {
	public:
		std::array<std::optional<hid_state>, faroela::api::port_max> hid_states;

		// TODO: Since this basically just acts as a store -- these don't need to be std::function; just a temporary fix.
		std::function<void(hid_status_event&)> status_callback;
		std::function<void(hid_button_event&)> button_callback;
		std::function<void(hid_axis_event&)> axis_callback;
	};
}
