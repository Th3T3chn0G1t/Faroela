// SPDX-License-Identifier: MIT

#pragma once

#ifdef _WIN32
# define FAROELA_COMMON_EXPORT __declspec(dllexport)
#else
# define FAROELA_COMMON_EXPORT [[gnu::visibility("default")]]
#endif

#include <algorithm>
#include <array>
#include <bit>
#include <concepts>
#include <expected>
#include <format>
#include <functional>
#include <memory>
#include <optional>
#include <source_location>
#include <span>
#include <string_view>
#include <system_error>
#include <utility>
#include <variant>

#include <cstdlib>
#include <cstddef>

#ifdef __APPLE__
# include <TargetConditionals.h>
#endif

#include <spdlog/spdlog.h>
#include <spdlog/stopwatch.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/msvc_sink.h>

#include <magic_enum/magic_enum.hpp>

#include <mapbox/eternal.hpp>

namespace faroela::common {
	template<typename type, typename... args>
	concept is_list_constructible = requires(args&&... v) {
			{ type{ std::forward<args>(v)... } };
	};

	// NOTE: From https://stackoverflow.com/a/57092496.
	template<typename type, typename... args>
	std::unique_ptr<type> make_unique(std::nothrow_t, args&&... v) noexcept(noexcept(type(std::forward<args>(v)...))) {
		return std::unique_ptr<type>(new(std::nothrow) type(std::forward<args>(v)...));
	}

	// NOTE: Only for allocating aggregate types. Clean up with `free`.
	template<typename type>
	type* typed_alloc() {
		return static_cast<type*>(malloc(sizeof(type)));
	}
}
