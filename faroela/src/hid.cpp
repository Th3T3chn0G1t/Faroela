// SPDX-License-Identifier: MIT

#include <faroela/hid.hpp>
#include <faroela/core.hpp>

namespace faroela {
	result<void> hid_system::create(context* ctx, hid_system& hid) {
		hid.ctx = ctx;

		hid.status_callback = [&hid](auto& event) {
			hid.ctx->logger->info("HID port '{}' {}", magic_enum::enum_name(event.port), event.connected ? "connected" : "disconnected");

			auto& state = hid.hid_states[static_cast<std::size_t>(magic_enum::enum_integer(event.port))];
#ifndef NDEBUG
			if(bool(state) == event.connected) {
				// TODO: EH.
			}
#endif

			if(event.connected) {
				state = hid_state{};
			}
			else {
				state = std::nullopt;
			}
		};

		hid.button_callback = [&hid](auto& event) {
			hid.ctx->logger->info("HID port '{}' button '{}' {}", magic_enum::enum_name(event.port), event.button, event.pressed ? "pressed" : "released");

			auto& state = hid.hid_states[static_cast<std::size_t>(magic_enum::enum_integer(event.port))];
#ifndef NDEBUG
			if(!state) {
				// TODO: EH.
			}
#endif

			state->buttons[event.button] = event.pressed;
		};

		hid.axis_callback = [&hid](auto& event) {
			hid.ctx->logger->info("HID port '{}' axis '{}' at '{}'", magic_enum::enum_name(event.port), magic_enum::enum_name(event.axis), event.value);

			auto& state = hid.hid_states[static_cast<std::size_t>(magic_enum::enum_integer(event.port))];
#ifndef NDEBUG
			if(!state) {
				// TODO: EH.
			}
#endif

			state->axes[static_cast<std::size_t>(magic_enum::enum_integer(event.axis))] = event.value;
		};

		auto result = ctx->add_system("hid");
		if(!result) [[unlikely]] {
			return forward(result);
		}

		return {};
	}
}
