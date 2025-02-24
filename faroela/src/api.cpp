// SPDX-License-Identifier: MIT

#include <faroela/core.hpp>
#include <faroela/hid.hpp>

#include <faroela/common/log.hpp>

#include <faroela/api/faroela.hpp>

extern "C" {
	namespace faroela::api {
		// TODO: Make common root API error state/log handover.

		FAROELA_EXPORT link_bool faroela_initialize(faroela::context** ctx, [[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
			const auto result = faroela::context::initialize();

			*ctx = result ? *result : nullptr;

			return result.has_value();
		}

		FAROELA_EXPORT void faroela_shutdown(faroela::context** ctx) {
			faroela::context::shutdown(*ctx);
		}

		FAROELA_EXPORT void faroela_log(faroela::context* ctx, verbosity level, const char* message) {
			ctx->client_logger->log(spdlog::level::level_enum(level), std::string_view(message));
		}

		FAROELA_EXPORT link_bool faroela_hid_status(faroela::context* ctx, hid::port port, link_bool connected) {
			// TODO: Wrap up this create+dispatch in a context method.
			const auto instance = faroela::delegate<faroela::hid_status_event>::create(ctx->hid.status_callback.value(), port, !!connected);
			if(!instance) [[unlikely]] {
				return false;
			}

			const auto result = ctx->submit("hid", **instance);

			return result.has_value();
		}

		FAROELA_EXPORT link_bool faroela_hid_button_event(faroela::context* ctx, hid::port port, hid::button button, link_bool pressed) {
			const auto instance = faroela::delegate<faroela::hid_button_event>::create(ctx->hid.button_callback.value(), port, button, !!pressed);
			if(!instance) [[unlikely]] {
				return false;
			}

			const auto result = ctx->submit("hid", **instance);

			return result.has_value();
		}

		FAROELA_EXPORT link_bool faroela_hid_axis_event(faroela::context* ctx, hid::port port, hid::axis axis, float value) {
			const auto instance = faroela::delegate<faroela::hid_axis_event>::create(ctx->hid.axis_callback.value(), port, axis, value);
			if(!instance) [[unlikely]] {
				return false;
			}

			const auto result = ctx->submit("hid", **instance);

			return result.has_value();
		}
	}
}
