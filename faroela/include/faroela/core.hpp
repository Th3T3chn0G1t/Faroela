// SPDX-License-Identifier: MIT

#ifdef _WIN32
# define FARO_WINDOWS
#endif

#ifdef FARO_WINDOWS
# define FARO_EXPORT __declspec(dllexport)
#else
# define FARO_EXPORT [[gnu::visibility("default")]]
#endif
