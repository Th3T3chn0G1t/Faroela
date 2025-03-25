// SPDX-License-Identifier: MIT

#ifndef FAROELA_COMMON_EXPORT
# define FAROELA_COMMON_EXPORT
#endif

namespace faroela {
	class context;
}

namespace faroela::api {
	using context = faroela::context;
	using link_bool = unsigned;
}
