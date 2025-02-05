// SPDX-License-Identifier: MIT

#pragma once

template<>
struct std::formatter<std::source_location> {
	template<class context>
	constexpr context::iterator parse(context& ctx) {
		return ctx.begin();
	}

	template<class context>
	context::iterator format(std::source_location location, context& ctx) const {
		std::string out = std::format("{}() at {}{}:{}", location.function_name(), location.file_name(), location.line(), location.column());
		return std::ranges::copy(std::move(out), ctx.out()).out;
	}
};
