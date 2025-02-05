// SPDX-License-Identifier: MIT

#pragma once

namespace faroela {
	enum class error_code {
		unknown_error
	};

	struct error {
		error_code code;
		std::source_location location;
		std::string_view message;

		explicit error(std::string_view message, error_code code = error_code::unknown_error) noexcept :
				code(code), location(std::source_location::current()), message(message) {}
	};

	// NOTE: For some reason `[[gnu::warn_unused_result]]` and `[[nodiscard]]` don't work here.
	template<typename T>
	using result = std::expected<T, error>;
}
