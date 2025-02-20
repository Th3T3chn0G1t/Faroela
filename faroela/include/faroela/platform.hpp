// SPDX-License-Identifier: MIT

#pragma once

#include <faroela/common/result.hpp>

namespace faroela {
	class context;

	[[nodiscard]]
	result<void> initialize_platform(context&);
}
