// SPDX-License-Identifier: MIT

#ifdef MAGIC_ENUM_VERSION_MAJOR
template<>
struct magic_enum::customize::enum_range<faroela::api::hid_button> {
	static constexpr int min = 0;
	static constexpr int max = faroela::api::button_max;
};
#endif
