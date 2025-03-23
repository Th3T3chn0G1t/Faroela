// SPDX-License-Identifier: MIT

#pragma once

#include <faroela/common/result.hpp>

template<>
struct std::formatter<std::source_location> {
	template<class context>
	constexpr context::iterator parse(context& ctx) {
		return ctx.begin();
	}

	template<class context>
	context::iterator format(std::source_location location, context& ctx) const {
		std::string out = std::format("'{}()' at {}:{}:{}", location.function_name(), location.file_name(), location.line(), location.column());
		return std::ranges::copy(std::move(out), ctx.out()).out;
	}
};

template<>
struct std::formatter<faroela::common::error> {
	template<class context>
	constexpr context::iterator parse(context& ctx) {
		return ctx.begin();
	}

	template<class context>
	context::iterator format(faroela::common::error error, context& ctx) const {
		std::string out = std::format("{} in {}: {}", magic_enum::enum_name(error.code), error.location, error.message);
		return std::ranges::copy(std::move(out), ctx.out()).out;
	}
};

template<>
struct std::formatter<faroela::common::unexpected> {
	template<class context>
	constexpr context::iterator parse(context& ctx) {
		return ctx.begin();
	}

	template<class context>
	context::iterator format(faroela::common::unexpected unexpected, context& ctx) const {
		std::string out = std::format("{}", unexpected.error());
		return std::ranges::copy(std::move(out), ctx.out()).out;
	}
};

template<typename type>
struct std::formatter<faroela::common::result<type>> {
	template<class context>
	constexpr context::iterator parse(context& ctx) {
		return ctx.begin();
	}

	template<class context>
	context::iterator format(faroela::common::result<type> result, context& ctx) const {
		std::string out;

		if(result) {
			out = "ok";
		}
		else {
			out = std::format("{}", result.error());
		}

		return std::ranges::copy(std::move(out), ctx.out()).out;
	}
};
