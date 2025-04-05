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

	class vfs_stream {
	private:
		uv_loop_t* loop;
		uv_file fd;
		uv_stat_t stat;

	public:
		result<void> close();

	public:
		vfs_stream(const vfs_stream&) = delete; // copy constructor
		vfs_stream& operator=(const vfs_stream&) = delete; // copy assignment

		vfs_stream(vfs_stream&&) = delete; // move constructor
		vfs_stream& operator=(vfs_stream&&) noexcept = delete; // move assignment
	};

	struct vfs_request {
		uv_fs_t handle;
	};

	struct vfs_open_event {
		using callable = tl::function_ref<void(vfs_stream)>;

		std::string path;
		vfs_request* request;
		callable callback;
	};

	struct vfs_read_event {
		using callable = tl::function_ref<void(void)>;

		// NOTE: Buffer is expected to live until callback returns.
		uv_buf_t buffer;
		// Called once the read is complete.
		callable callback;
	};

	// NOTE: VFS does not perform internal caching.
	class vfs_system {
	private:
		context* ctx;

		std::vector<vfs_source> sources;

		bool mapped_reads;

	public:
		std::function<void(vfs_open_event&)> open_callback;
		std::function<void(vfs_read_event&)> read_callback;

	public:
		vfs_system() = default;

		[[nodiscard]]
		static result<void> create(context*, bool, vfs_system&);

	public:
		vfs_system(const vfs_system&) = delete; // copy constructor
		vfs_system& operator=(const vfs_system&) = delete; // copy assignment

		vfs_system(vfs_system&&) = delete; // move constructor
		vfs_system& operator=(vfs_system&&) noexcept = delete; // move assignment

	public:
		// NOTE: Sources are searched in the order they are added.
		[[nodiscard]]
		result<void> add_archive_source(std::string_view);

		[[nodiscard]]
		result<void> add_native_source(std::string_view);
	};
}
