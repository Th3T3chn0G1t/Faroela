// SPDX-License-Identifier: MIT

#pragma once

#include <uv.h>

namespace faroela {
	// TODO: Empty event type should be able to specialize to void.
	struct delegate_dummy {};

	template<typename event_type, typename signature = void(event_type&)>
	struct delegate_storage {
		// TODO: Replace once std::function_ref becomes more widespread.
		// TODO: Should this signature return a result?
		using callable = tl::function_ref<signature>;

		callable callback;
		event_type data;
	};

	template<typename event_type, bool destroy_handle = true, typename handle_type = uv_async_t, typename signature = void(event_type&)>
	class delegate {
	public:
		using storage_type = delegate_storage<event_type, signature>;
		using callable = storage_type::callable;
		using self_type = delegate<event_type, destroy_handle, handle_type, signature>;
		using event = event_type;

	public:
		storage_type storage;

	public:
		template<typename... args>
		[[nodiscard]]
		static result<self_type*> create(callable callback, args&&... v) {
			// TODO: Could this be pulled from a pool? May be better to do so from the ctx create wrapper.
			auto p = new(std::nothrow) self_type{ { callback, { std::forward<args>(v)... } } };
			if(!p) [[unlikely]] {
				return unexpect("failed to allocate delegate", error_code::out_of_memory);
			}

			return p;
		}

		static void call(handle_type* handle) noexcept {
			auto data = static_cast<self_type*>(handle->data);
			if constexpr(std::invocable<callable, event_type, handle_type*>) {
				data->storage.callback(data->storage.data, handle);
			}
			else {
				data->storage.callback(data->storage.data);
			}

			if constexpr(destroy_handle) {
				delete data;

				// TODO: Cloned from ctx -- this should just be part of the handle wrapper.
				uv_close(reinterpret_cast<uv_handle_t*>(handle), reinterpret_cast<uv_close_cb>(free));
			}
		}
	};

	using idle_delegate = delegate<delegate_dummy, false, uv_idle_t>;
}
