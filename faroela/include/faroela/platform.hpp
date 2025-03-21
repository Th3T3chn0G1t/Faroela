// SPDX-License-Identifier: MIT

#pragma once

#include <faroela/common/result.hpp>

#include <faroela/api/fwd.hpp>

namespace faroela {
	[[nodiscard]]
	result<void> initialize_platform(context&);
}
