// SPDX-License-Identifier: MIT

#pragma once

#include <algorithm>
#include <expected>
#include <format>
#include <memory>
#include <source_location>
#include <string_view>

#include <spdlog/spdlog.h>
#include <spdlog/stopwatch.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/msvc_sink.h>

#include <magic_enum/magic_enum.hpp>
