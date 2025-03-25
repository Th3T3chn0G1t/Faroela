// SPDX-License-Identifier: MIT

#pragma once

#include <faroela/common/result.hpp>

#include <faroela/core.hpp>

namespace faroela {
	[[nodiscard]]
	result<void> initialize_platform(context&);
}
