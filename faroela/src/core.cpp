#include <faroela/core.hpp>

namespace faroela {
	extern "C" FARO_EXPORT void faro_export_test(const char* message) {
		std::cout << message << std::endl;
	}
}
