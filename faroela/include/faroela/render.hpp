// SPDX-License-Identifier: MIT

#pragma once

#include <faroela/common/result.hpp>

#include <faroela/delegate.hpp>

namespace faroela {
	class context;

	// TODO: This (and really all homogenous state classes like it) need to have their
	//  	 Move and copy [assignment] constructors deleted. In this particular case this also needs a destructor.
	class render_system {
	private:
		context* ctx;

		// TODO: Find better internal way to store these.
		std::function<void(delegate_dummy&)> idle_callback;
		std::function<void(delegate_dummy&)> attach_callback;

		delegate<delegate_dummy, false>* worker;

		std::array<int, 2> offset;
		std::array<unsigned, 2> resolution;

	public:
		static result<render_system> create(context*);

	public:
		void clip(int, int, unsigned, unsigned);
		result<void> attach(void*, void*, void*);

		result<void> update(bool = true);
	};
}
