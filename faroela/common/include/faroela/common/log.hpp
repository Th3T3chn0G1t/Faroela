// SPDX-License-Identifier: MIT

#pragma once

#include <faroela/common/result.hpp>
#include <faroela/common/formatters.hpp>

namespace faroela::common {
	using logger = std::shared_ptr<spdlog::logger>;

	void register_default_loggers(const spdlog::filename_t&);

	void log_error(logger, const error&);

	// TODO: Just define custom formatter for error/result instead of having these bespoke functions.
	inline void log_error(logger log, const error& error) {
		log->error("{} in {}: {}", magic_enum::enum_name(error.code), error.location, error.message);
	}

	inline void log_error(logger log, const unexpected& error) {
		log_error(log, error.error());
	}

	template<typename expected>
	bool log_result(logger log, const result<expected>& result) {
		if(!result) log_error(log, result.error());

		return result.has_value();
	}
}
