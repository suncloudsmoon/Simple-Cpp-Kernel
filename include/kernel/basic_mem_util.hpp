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
	enum {
		ALLOC_SUCCESS = 0,
		ALLOC_INVALID_SIZE = -1,
		ALLOC_MEM_ERR = -2,
		ALLOC_TRACKER_FAILURE = -3
	};
	enum {
		REALLOC_SUCCESS = 0,
		REALLOC_INVALID_ARG = -1,
		REALLOC_NULL_MALLOC = -2,
		REALLOC_MEMCPY_FAILURE = -3
	};
	struct blk {
		blk(void *pointer = nullptr, size_t length = 0) : ptr(pointer), len(length) {}
		void *ptr;
		size_t len;
	};
	void mem_init();
	zl::expected<blk> alloc(size_t size);
	zl::expected<blk> realloc(void *src, size_t size);
	bool free(void *ptr);
}

#endif /* KERNEL_BASIC_MEM_UTIL */