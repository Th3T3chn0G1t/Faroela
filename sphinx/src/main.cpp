// SPDX-License-Identifier: MIT

#include <faroela-api/faroela.hpp>

int main() {
	faroela_api::faro_initialize();

	faroela_api::faro_log(faroela_api::info, "Hello, Sphinx!");

	faroela_api::faro_shutdown();

	return 0;
}
