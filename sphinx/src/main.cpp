// SPDX-License-Identifier: MIT

#include <faroela-api/faroela.hpp>

int main() {
	faroela_api::context* ctx;

	if(!faroela_api::faroela_initialize(&ctx)) return 1;

	faroela_api::faroela_log(ctx, faroela_api::info, "Hello, Sphinx!");

	faroela_api::faroela_shutdown(&ctx);

	return 0;
}
