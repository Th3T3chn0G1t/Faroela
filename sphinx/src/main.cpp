// SPDX-License-Identifier: MIT

#include <faroela/api/faroela.hpp>

int main(int argc, char** argv) {
	faroela::api::context* ctx;

	if(!faroela::api::faroela_initialize(&ctx, argc, argv)) return 1;

	faroela::api::faroela_log(ctx, faroela::api::info, "Hello, Sphinx!");

	faroela::api::faroela_shutdown(&ctx);

	return 0;
}

[[maybe_unused]]
int WinMain(void*, void*, char*, int) {
	return main(__argc, __argv);
}
