// SPDX-License-Identifier: MIT

#pragma once

#include <faroela/delegate.hpp>

#include <faroela/api/faroela.hpp>

namespace faroela {
	class context;

	struct hid_status_event {
		faroela::api::hid_port port;
		bool connected;
	};

	struct hid_button_event {
		faroela::api::hid_port port;
		unsigned button;
		bool pressed;
	};

	struct hid_axis_event {
		faroela::api::hid_port port;
		faroela::api::hid_axis axis;
		float value;
	};

	struct hid_state {
		// TODO: This is pretty janky -- ideally we should have a HID registry and only track the state of buttons
		//		 Registered in there but this will do for now.
		std::array<bool, std::numeric_limits<unsigned short>::max()> buttons{};
		std::array<float, magic_enum::enum_integer(faroela::api::hid_axis::max)> axes{};
	};

	class hid_system {
	private:
		context* ctx;

	public:
		std::array<std::optional<hid_state>, magic_enum::enum_integer(faroela::api::hid_port::max)> hid_states;

		// TODO: Since this basically just acts as a store -- these don't need to be std::function; just a temporary fix.
		std::function<void(hid_status_event&)> status_callback;
		std::function<void(hid_button_event&)> button_callback;
		std::function<void(hid_axis_event&)> axis_callback;

	public:
		hid_system() = default;

		[[nodiscard]]
		static result<void> create(context*, hid_system&);

	public:
		hid_system(const hid_system&) = delete; // copy constructor
		hid_system& operator=(const hid_system&) = delete; // copy assignment

		hid_system(hid_system&&) = delete; // move constructor
		hid_system& operator=(hid_system&&) noexcept = delete; // move assignment
	};
}
