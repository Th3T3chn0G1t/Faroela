// SPDX-License-Identifier: MIT

#include <faroela/render.hpp>
#include <faroela/delegate.hpp>
#include <faroela/core.hpp>

namespace faroela {
	result<render_system> render_system::create(context* ctx) {
		render_system retval{};

		retval.ctx = ctx;

		bgfx::renderFrame();

		auto result = ctx->add_system("render");
		if(!result) {
			return forward(result);
		}

		return retval;
	}

	void render_system::clip(int x, int y, unsigned width, unsigned height) {
		offset = { { x, y } };
		resolution = { { width, height } };
		// TODO: Need to send Rect/Scissor event command if RT is started.
	}

	result<void> render_system::attach(void* handle, void* connection, void* context) {
		// TODO: On receive shutdown render event, remove this idler and dispatch final teardown handler.
		idle_callback = [](auto&) {
			bgfx::touch(0);
			bgfx::frame();
		};

		auto worker_result = delegate<delegate_dummy, false>::create(idle_callback);
		if(!worker_result) {
			return forward(worker_result);
		}

		worker = *worker_result;

		attach_callback = [this, handle, connection, context](auto&) noexcept {
			bgfx::Init init_data;

			init_data.platformData.nwh = handle;
			init_data.platformData.ndt = connection;
			init_data.platformData.context = context;

			init_data.resolution.width = resolution[0];
			init_data.resolution.height = resolution[1];

			// TODO: Set reset/sync and API from client.

			// TODO: Stub EH and callback interface. Can we get sensible errors out of init failure?
			// TODO: This hangs if init fails on main thread -- need a way to run a single frame pump.
			bgfx::init(init_data);

			auto result = ctx->add_idler("render", worker);
			if(!result) {
				// TODO: EH?
			}
		};

		auto result = ctx->submit<delegate_dummy>("render", attach_callback);
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
