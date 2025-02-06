// SPDX-License-Identifier: MIT

#pragma once

namespace faroela {
	enum class error_code {
		unknown_error,
		out_of_memory
	};

	struct error {
		error_code code;
		std::source_location location;
		std::string_view message;

		explicit error(std::string_view, error_code, std::source_location);
	};

	// NOTE: For some reason `[[gnu::warn_unused_result]]` and `[[nodiscard]]` don't work here.
	template<typename T>
	using result = std::expected<T, error>;

	constexpr inline std::unexpected<error> unexpected(const std::string_view message, error_code code = error_code::unknown_error, std::source_location location = std::source_location::current()) {
		return std::unexpected<error>(error(message, code, location));
	}
}
