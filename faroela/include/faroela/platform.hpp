// SPDX-License-Identifier: MIT

#pragma once

#include <faroela/result.hpp>

namespace faroela {
	class context;

	result<void> initialize_platform(context&);
}
