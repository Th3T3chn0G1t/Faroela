// SPDX-License-Identifier: MIT

#include <faroela-api/faroela.hpp>

int main() {
	faroela_api::faroela_initialize();

	faroela_api::faroela_log(faroela_api::info, "Hello, Sphinx!");

	faroela_api::faroela_shutdown();

	return 0;
}
