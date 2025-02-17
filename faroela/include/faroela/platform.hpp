// SPDX-License-Identifier: MIT

#pragma once

#include <faroela/common/result.hpp>

namespace faroela {
	class context;

	result<void> initialize_platform(context&);
}
