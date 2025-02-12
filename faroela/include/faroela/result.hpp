// SPDX-License-Identifier: MIT

#pragma once

namespace faroela {
	// TODO: Should create a `std::error_code`/`category` adapter.
	enum class error_code {
		unknown_error,
		out_of_memory
	};

	struct error {
		error_code code;
		std::source_location location;
		std::string message;
	};

	// NOTE: For some reason `[[gnu::warn_unused_result]]` and `[[nodiscard]]` don't work here.
	template<typename T>
	using result = std::expected<T, error>;

	using unexpected = std::unexpected<error>;

	// TODO: Allow move of string parameter into error struct for system message.
	constexpr inline unexpected unexpect(const std::string_view message, error_code code = error_code::unknown_error, std::source_location location = std::source_location::current()) {
		return unexpected(error{ code, location, std::string(message) });
	}

	constexpr inline unexpected system_error(int code, std::source_location location = std::source_location::current()) {
		std::error_code sys(code, std::system_category());
		// TODO: Offer `strerrorname_np` where available to convert code name.
		return unexpect(sys.message(), error_code::unknown_error, location);
	}

	constexpr inline unexpected forward(const error& fwd) {
		return unexpected(fwd);
	}

	template<typename T>
	constexpr inline unexpected forward(const result<T>& result) {
		return forward(result.error());
	}
}
