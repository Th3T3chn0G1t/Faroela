// SPDX-License-Identifier: MIT

#pragma once

#include <faroela/common/result.hpp>
#include <faroela/common/formatters.hpp>

namespace faroela::common {
	using logger = std::shared_ptr<spdlog::logger>;

	void register_default_loggers(const spdlog::filename_t&);
}
