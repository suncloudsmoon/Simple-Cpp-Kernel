/*
 * Copyright (c) 2022, suncloudsmoon and the Simple-Cpp-Kernel contributors.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef KERNEL_BASIC_MEM_UTIL
#define KERNEL_BASIC_MEM_UTIL

#include <stddef.h>
#include <lib/zutil.hpp>

namespace os {
	namespace mem {
		constexpr size_t dyn_alloc_loc = 0x9770;
		namespace alloc_code {
			enum {
				success = 0,
				invalid_size = -1,
				mem_err = -2,
				tracker_fail = -3
			};
		}
		namespace realloc_code {
			enum {
				success = 0,
				invalid_arg = -1,
				null_malloc = -2,
				memcpy_fail = -3
			};
		}
		struct blk {
			blk(void *pointer = nullptr, size_t length = 0) : ptr(pointer), len(length) {}
			void *ptr;
			size_t len;
		};
		void init();
		zl::expected<blk> alloc(size_t size);
		zl::expected<blk> realloc(void *src, size_t size);
		bool free(void *ptr);
	}
}

#endif /* KERNEL_BASIC_MEM_UTIL */