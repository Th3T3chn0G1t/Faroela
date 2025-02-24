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

		std::optional<delegate<hid_status_event>::callable> status_callback;
		std::optional<delegate<hid_button_event>::callable> button_callback;
		std::optional<delegate<hid_axis_event>::callable> axis_callback;
	};
}
