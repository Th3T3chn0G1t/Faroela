// SPDX-License-Identifier: MIT

#include <faroela/core.hpp>
#include <faroela/hid.hpp>

#include <faroela/common/log.hpp>

#include <faroela/api/faroela.hpp>

namespace faroela::api {
	template<typename type>
	static unsigned handle_result(faroela::context* ctx, const result<type>& result) {
		if(!result) [[unlikely]] {
			ctx->api_logger->error("{}", result);
			return false;
		}

		return true;
	}
}

extern "C" {
	namespace faroela::api {
		// TODO: Make common root API error state/log handover.
		FAROELA_COMMON_EXPORT unsigned faroela_initialize(faroela::context** ctx, int, char**) {
			const auto result = faroela::context::initialize();

			*ctx = result ? *result : nullptr;

			return result.has_value();
		}

		FAROELA_COMMON_EXPORT unsigned faroela_shutdown(faroela::context** ctx) {
			faroela::context::shutdown(*ctx);

			return true;
		}

		FAROELA_COMMON_EXPORT unsigned faroela_log(faroela::context* ctx, verbosity level, const char* message) {
			ctx->client_logger->log(spdlog::level::level_enum(level), std::string_view(message));

			return true;
		}

		FAROELA_COMMON_EXPORT unsigned faroela_hid_status(faroela::context* ctx, hid_port port, unsigned connected) {
			return handle_result(ctx, ctx->submit<delegate<faroela::hid_status_event>>("hid", ctx->hid.status_callback, port, !!connected));
		}

		FAROELA_COMMON_EXPORT unsigned faroela_hid_button_event(faroela::context* ctx, hid_port port, unsigned button, unsigned pressed) {
			return handle_result(ctx, ctx->submit<delegate<faroela::hid_button_event>>("hid", ctx->hid.button_callback, port, button, !!pressed));
		}

		FAROELA_COMMON_EXPORT unsigned faroela_hid_axis_event(faroela::context* ctx, hid_port port, hid_axis axis, float value) {
			return handle_result(ctx, ctx->submit<delegate<faroela::hid_axis_event>>("hid", ctx->hid.axis_callback, port, axis, value));
		}

		FAROELA_COMMON_EXPORT unsigned faroela_render_attach(faroela::context* ctx, void* handle, void* connection, void* context) {
			return handle_result(ctx, ctx->submit<delegate<faroela::render_attach_event>>("render", ctx->render.attach_callback, handle, connection, context));
		}

		FAROELA_COMMON_EXPORT unsigned faroela_render_detach(faroela::context* ctx) {
			return handle_result(ctx, ctx->render.destroy());
		}

		FAROELA_COMMON_EXPORT unsigned faroela_render_clip(faroela::context* ctx, int x, int y, unsigned width, unsigned height) {
			return handle_result(ctx, ctx->submit<delegate<faroela::render_clip_event>>("render", ctx->render.clip_callback, x, y, width, height));
		}

		FAROELA_COMMON_EXPORT unsigned faroela_render_update(faroela::context* ctx, unsigned wait) {
			return handle_result(ctx, ctx->render.update(!!wait));
		}
	}
}
