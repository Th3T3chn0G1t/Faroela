// SPDX-License-Identifier: MIT

#pragma once

#include <faroela/common/pch.hpp>
#include <faroela/common/result.hpp>

#include <ankerl/unordered_dense.h>

#include <tl/function_ref.hpp>

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

// TODO: We may want to provide a fallback API from libuv to support WASM targeting
//		 Unless someone wants to spend a few months hamfisting libuv into emscripten.
#include <uv.h>

#include <entt/entt.hpp>

namespace faroela {
	using common::result;
	using error_code = common::error_code;

	using namespace ankerl;
}
