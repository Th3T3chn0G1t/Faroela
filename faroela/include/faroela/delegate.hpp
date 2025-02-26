// SPDX-License-Identifier: MIT

#pragma once

#include <uv.h>

namespace faroela {
	template<typename event_type>
	class delegate {
	public:
		// TODO: Once std::function_ref becomes more widespread, replace all tl::function_ref.
		// TODO: Should this signature return a result?
		using callable = tl::function_ref<void(event_type&)>;

	private:
		std::optional<callable> callback;
		event_type data;

		template<typename... args>
		delegate(callable proc, args&&... v) : callback{ proc }, data{ std::forward<args>(v)... } {}

	public:
		template<typename... args>
		[[nodiscard]]
		static result<delegate<event_type>*> create(callable callback, args&&... v) {
			// TODO: Could this be pulled from a pool? May be better to do so from the ctx create wrapper.
			auto p = new(std::nothrow) delegate<event_type>{ callback, event_type{ std::forward<args>(v)... } };

			if(!p) {
				return unexpect("failed to allocate delegate", error_code::out_of_memory);
			}

			return p;
		}

		static void call(uv_handle_t* handle) noexcept {
			auto data = static_cast<delegate<event_type>*>(handle->data);
			data->callback.value()(data->data);
			delete data;

			uv_close(handle, nullptr);
			delete handle;
		}

		static void call(uv_async_t* handle) noexcept {
			call(reinterpret_cast<uv_handle_t*>(handle));
		}
	};
}
