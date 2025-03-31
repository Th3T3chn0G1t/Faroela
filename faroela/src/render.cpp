// SPDX-License-Identifier: MIT

#include <faroela/render.hpp>
#include <faroela/delegate.hpp>
#include <faroela/core.hpp>

namespace faroela {
	result<void> render_system::create(context* ctx, render_system& render) {
		render.ctx = ctx;

		bgfx::renderFrame();

		// TODO: On receive shutdown render event, remove this idler and dispatch final teardown handler.
		render.idle_callback = [](auto&) {
			bgfx::touch(0);
			bgfx::frame();
		};

		auto worker_result = delegate<delegate_dummy, false>::create(render.idle_callback);
		if(!worker_result) {
			return forward(worker_result);
		}

		render.worker = *worker_result;

		render.attach_callback = [&render](auto& event) noexcept {
			bgfx::Init init_data;

			init_data.platformData.nwh = event.handle;
			init_data.platformData.ndt = event.connection;
			init_data.platformData.context = event.context;

			init_data.resolution.width = render.resolution[0];
			init_data.resolution.height = render.resolution[1];

			// TODO: Set reset/sync and API from client.

			// TODO: Stub EH and callback interface. Can we get sensible errors out of init failure?
			// TODO: This hangs if init fails on main thread -- need a way to run a single frame pump.
			bgfx::init(init_data);

			auto result = render.ctx->add_idler("render", render.worker);
			if(!result) {
				// TODO: EH?
			}
		};

		render.clip_callback = [&render](auto& event) {
			render.offset = { { event.x, event.y } };
			render.resolution = { { event.width, event.height } };
			// TODO: Need to send Rect/Scissor command if RT is started.
		};

		auto result = ctx->add_system("render");
		if(!result) {
			return forward(result);
		}

		return {};
	}

	result<void> render_system::update(bool wait_frame) {
		bgfx::renderFrame(wait_frame ? -1 : 0);

		return {};
	}
}
