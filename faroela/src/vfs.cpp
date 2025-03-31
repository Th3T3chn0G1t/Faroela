// SPDX-License-Identifier: MIT

#pragma once

#include <faroela/vfs.hpp>
#include <faroela/core.hpp>

namespace faroela {
	result<void> vfs_system::create(context* ctx, bool mapped_reads, vfs_system& vfs) {
		vfs.ctx = ctx;
		// NOTE: (From the libuv docs):
		// 		 > On Windows, under non-MSVC environments (e.g. when GCC or Clang is used to build libuv),
		// 		 > Files opened using UV_FS_O_FILEMAP may cause a fatal crash if the memory mapped read operation fails.
		// TODO: We probably want to automatically disable this under that case.
		vfs.mapped_reads = mapped_reads;

		vfs.read_callback = [](auto&) {
			//ctx->get_system("vfs")
		};

		// TODO: Want a separate system for the "main" vfs and archive/load dispatch as libzip expects reads
		//		 To be synchronous. Possibly one system per-archive to avoid lockups.
		//		 See `zip_source_file_common.c:read_file` for "expected" source behaviour.
		auto result = ctx->add_system("vfs");
		if(!result) {
			return forward(result);
		}

		return {};
	}

	result<void> vfs_system::add_archive_source(std::string_view) {
		return unexpect("archive vfs sources are not yet supported", error_code::not_implemented);
	}

	result<void> vfs_system::add_native_source(std::string_view path) {
		sources.emplace_back(vfs_native_source{ std::string(path) });

		return {};
	}
}
