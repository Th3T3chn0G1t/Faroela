// SPDX-License-Identifier: MIT

#include <faroela/platform.hpp>
#include <faroela/core.hpp>

#include <faroela/common/log.hpp>

#include <windows.h>

namespace faroela {
	result<void> initialize_platform([[maybe_unused]] context& ctx) {
		// TODO: Should this be part of common?
#if _WIN32_WINNT >= 0x0501
		if(!AttachConsole(ATTACH_PARENT_PROCESS)) {
			common::log_error(ctx.logger, common::system_error(GetLastError()));
		}
#endif

		return {};
	}
}
