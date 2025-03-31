// SPDX-License-Identifier: MIT

#pragma once

#include <faroela/common/result.hpp>

#include <faroela/delegate.hpp>

namespace faroela {
	class context;

	struct vfs_archive_source {

	};

	struct vfs_native_source {
		std::string base;
	};

	using vfs_source = std::variant<vfs_archive_source, vfs_native_source>;

	struct vfs_read_event {
		using callable = tl::function_ref<void(void)>;

		std::string path;
		// NOTE: Buffer is expected to live until callback returns.
		uv_buf_t buffer;
		// Called once the read is complete.
		callable callback;
	};

	// NOTE: VFS does not perform internal caching.
	class vfs_system {
	private:
		context* ctx;

		std::function<void(vfs_read_event&)> read_callback;

		std::vector<vfs_source> sources;

		bool mapped_reads;

	public:
		vfs_system() = default;

		static result<void> create(context*, bool, vfs_system&);

	public:
		vfs_system(const vfs_system&) = delete; // copy constructor
		vfs_system& operator=(const vfs_system&) = delete; // copy assignment

		vfs_system(vfs_system&&) = delete; // move constructor
		vfs_system& operator=(vfs_system&&) noexcept = delete; // move assignment

	public:
		// NOTE: Sources are searched in the order they are added.
		result<void> add_archive_source(std::string_view);
		result<void> add_native_source(std::string_view);
	};
}
