// SPDX-License-Identifier: MIT

#pragma once

#include <faroela/common/result.hpp>

#include <faroela/delegate.hpp>

namespace faroela {
	class context;

	struct render_attach_event {
		void* handle;
		void* connection;
		void* context;
	};

	struct render_clip_event {
		int x;
		int y;
		unsigned width;
		unsigned height;
	};

	class render_system {
	private:
		context* ctx;

		uv_idle_t* idle_handle;
		idle_delegate* worker;

		std::array<int, 2> offset;
		std::array<unsigned, 2> resolution;

	public:
		// TODO: Find better internal way to store these.
		std::function<void(delegate_dummy&)> idle_callback;
		std::function<void(render_attach_event&)> attach_callback;
		std::function<void(render_clip_event&)> clip_callback;
		std::function<void(delegate_dummy&)> shutdown_callback;

	public:
		render_system() = default;

		[[nodiscard]]
		static result<void> create(context*, render_system&);

		[[nodiscard]]
		result<void> destroy();

	public:
		render_system(const render_system&) = delete; // copy constructor
		render_system& operator=(const render_system&) = delete; // copy assignment

		render_system(render_system&&) = delete; // move constructor
		render_system& operator=(render_system&&) noexcept = delete; // move assignment

	public:
		// NOTE: Must be called from main thread.
		[[nodiscard]]
		result<void> update(bool = true);
	};
}
