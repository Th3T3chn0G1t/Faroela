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
#include <string_view>
#include <system_error>
#include <utility>
#include <variant>

#include <cstdlib>

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
}
