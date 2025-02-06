// SPDX-License-Identifier: MIT

#pragma once

#include <faroela/result.hpp>

namespace faroela {
	void make_default_loggers(const spdlog::filename_t&);

	void log_error(std::shared_ptr<spdlog::logger>, const error&);

	template<typename T>
	bool log_result(std::shared_ptr<spdlog::logger> logger, const result<T>& result) {
		if(!result) log_error(logger, result.error());

		return result.has_value();
	}
}
