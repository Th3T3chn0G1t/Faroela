// SPDX-License-Identifier: MIT

#pragma once

namespace faroela::common {
	// TODO: Should create a `std::error_code`/`category` adapter.
	enum class error_code {
		unknown_error,
		out_of_memory,
		key_not_found,
		key_exists
	};

	struct error {
		// TODO: Variant of error enums to include external codes (?).
		// TODO: Should this also attach the failing call -- mostly for system/vendor errors?
		error_code code;
		std::source_location location;
		std::string message;
	};

	// NOTE: For some reason `[[gnu::warn_unused_result]]` and `[[nodiscard]]` don't work here.
	template<typename type>
	using result = std::expected<type, error>;

	using unexpected = std::unexpected<error>;

	// TODO: Allow move of string parameter into error struct for system message.
	[[nodiscard]]
	constexpr inline unexpected unexpect(const std::string_view message, error_code code = error_code::unknown_error, std::source_location location = std::source_location::current()) {
		return unexpected(error{ code, location, std::string(message) });
	}

	template<typename type>
	[[nodiscard]]
	constexpr inline unexpected system_error(type code, std::source_location location = std::source_location::current()) {
		std::error_code sys(static_cast<int>(code), std::system_category());
		// TODO: Offer `strerrorname_np` where available to convert code name.
		return unexpect(sys.message(), error_code::unknown_error, location);
	}

	[[nodiscard]]
	constexpr inline unexpected forward(const error& fwd) {
		return unexpected(fwd);
	}

	template<typename T>
	[[nodiscard]]
	constexpr inline unexpected forward(const result<T>& result) {
		return forward(result.error());
	}
}
