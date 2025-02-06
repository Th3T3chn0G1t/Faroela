// SPDX-License-Identifier: MIT

#include <faroela/result.hpp>

namespace faroela {
	error::error(std::string_view message, error_code code, std::source_location location) :
			code(code), location(location), message(message) {}
}
