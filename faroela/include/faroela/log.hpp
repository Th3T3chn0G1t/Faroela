// SPDX-License-Identifier: MIT

#pragma once

#include <faroela/result.hpp>

namespace faroela {
	void make_default_loggers(const spdlog::filename_t&);

	void client_log(spdlog::level::level_enum, std::string_view);

	void log_error(const error&);

	template<typename T>
	bool log_result(const result<T>& result) {
		if(!result) log_error(result.error());

		return result.has_value();
	}
}
