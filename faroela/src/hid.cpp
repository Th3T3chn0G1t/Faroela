// SPDX-License-Identifier: MIT

#include <faroela/hid.hpp>
#include <faroela/core.hpp>

namespace faroela {
	void hid_state::set_button(faroela::api::hid_button button, bool pressed) {
		buttons[static_cast<unsigned>(magic_enum::enum_integer(button))] = pressed;
	}

	void hid_state::set_axis(faroela::api::hid_axis axis, float value) {
		axes[static_cast<unsigned>(magic_enum::enum_integer(axis))] = value;
	}

	result<void> hid_system::create(context* ctx, hid_system& hid) {
		hid.ctx = ctx;

		hid.status_callback = [&hid](auto& event) {
			hid.ctx->logger->info("HID port '{}' {}", magic_enum::enum_name(event.port), event.connected ? "connected" : "disconnected");
		};

		hid.button_callback = [&hid](auto& event) {
			hid.ctx->logger->info("HID port '{}' button '{}' {}", magic_enum::enum_name(event.port), magic_enum::enum_name(event.button), event.pressed ? "pressed" : "released");
		};

		hid.axis_callback = [&hid](auto& event) {
			hid.ctx->logger->info("HID port '{}' axis '{}' at '{}'", magic_enum::enum_name(event.port), magic_enum::enum_name(event.axis), event.value);
		};

		auto result = ctx->add_system("hid");
		if(!result) [[unlikely]] {
			return forward(result);
		}

		return {};
	}
}
