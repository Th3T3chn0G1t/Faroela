// SPDX-License-Identifier: MIT

#pragma once

#include <uv.h>

namespace faroela {
	// TODO: Empty event type should be able to specialize to void.
	struct delegate_dummy {};

	template<typename event_type>
	struct delegate_storage {
		// TODO: Replace once std::function_ref becomes more widespread.
		// TODO: Should this signature return a result?
		using callable = tl::function_ref<void(event_type&)>;

		callable callback;
		event_type data;
	};

	template<typename event_type, bool destroy_handle = true>
	class delegate {
	public:
		using callable = delegate_storage<event_type>::callable;

	private:
		delegate_storage<event_type> storage;

	private:
		template<typename... args>
		requires(!std::is_same_v<event_type, void>)
		delegate(callable proc, args&&... v) : storage{ proc, { std::forward<args>(v)... } } {}

		delegate(callable proc) : storage{ proc } {}

	public:
		template<typename... args>
		[[nodiscard]]
		static result<delegate<event_type, destroy_handle>*> create(callable callback, args&&... v) {
			// TODO: Could this be pulled from a pool? May be better to do so from the ctx create wrapper.
			auto p = new(std::nothrow) delegate<event_type, destroy_handle>{ callback, event_type{ std::forward<args>(v)... } };

			if(!p) [[unlikely]] {
				return unexpect("failed to allocate delegate", error_code::out_of_memory);
			}

			return p;
		}

		static void call(uv_handle_t* handle) noexcept {
			auto data = static_cast<delegate<event_type, destroy_handle>*>(handle->data);
			data->storage.callback(data->storage.data);

			if constexpr(destroy_handle) {
				delete data;

				// TODO: Cloned from ctx -- this should just be part of the handle wrapper.
				uv_close(handle, reinterpret_cast<uv_close_cb>(free));
			}
		}

		static void call(uv_async_t* handle) noexcept { call(std::bit_cast<uv_handle_t*>(handle)); }
		static void call(uv_idle_t* handle) noexcept { call(std::bit_cast<uv_handle_t*>(handle)); }
	};
}
