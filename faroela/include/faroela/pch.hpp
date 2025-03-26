// SPDX-License-Identifier: MIT

#pragma once

#include <faroela/common/pch.hpp>
#include <faroela/common/result.hpp>

#include <ankerl/unordered_dense.h>

#include <tl/function_ref.hpp>

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#include <uv.h>

namespace faroela {
	using common::result;
	using error_code = common::error_code;

	using namespace ankerl;
}
