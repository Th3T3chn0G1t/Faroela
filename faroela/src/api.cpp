// SPDX-License-Identifier: MIT

#include <faroela/core.hpp>
#include <faroela/log.hpp>

#include <faroela-api/faroela.hpp>

extern "C" {
	namespace faroela_api {
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

		FAROELA_EXPORT void faroela_hid_status(faroela::context* ctx, hid::port port, link_bool connected) {
			(void) ctx;
			(void) port;
			(void) connected;
		}

		FAROELA_EXPORT void faroela_hid_button_event(faroela::context* ctx, hid::port port, hid::button button, link_bool pressed) {
			(void) ctx;
			(void) port;
			(void) button;
			(void) pressed;
		}

		FAROELA_EXPORT void faroela_hid_axis_event(faroela::context* ctx, hid::port port, hid::axis axis, float value) {
			(void) ctx;
			(void) port;
			(void) axis;
			(void) value;
		}
	}
}
