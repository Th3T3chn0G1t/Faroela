// SPDX-License-Identifier: MIT

#pragma once

#include <faroela/result.hpp>

namespace faroela {
	using logger = std::shared_ptr<spdlog::logger>;

	void make_default_loggers(const spdlog::filename_t&);

	void log_error(logger, const error&);

	// TODO: Just define custom formatter for error/result instead of having these bespoke functions.
	inline void log_error(logger log, const unexpected& error) {
		log_error(log, error.error());
	}

	template<typename T>
	bool log_result(logger log, const result<T>& result) {
		if(!result) log_error(log, result.error());

		return result.has_value();
	}
}
