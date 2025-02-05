// SPDX-License-Identifier: MIT

#pragma once

#ifdef _WIN32
# define FARO_EXPORT __declspec(dllexport)
#else
# define FARO_EXPORT [[gnu::visibility("default")]]
#endif

#include <faroela/result.hpp>

namespace faroela {
	result<void> initialize();
	void shutdown();
}
