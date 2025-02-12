// SPDX-License-Identifier: MIT

#include <faroela/platform.hpp>
#include <faroela/log.hpp>
#include <faroela/core.hpp>

#include <windows.h>

namespace faroela {
	result<void> initialize_platform([[maybe_unused]] context& ctx) {
#if _WIN32_WINNT >= 0x0501
		if(!AttachConsole(ATTACH_PARENT_PROCESS)) {
			log_error(ctx.logger, system_error(GetLastError()));
		}
#endif

		return {};
	}
}
