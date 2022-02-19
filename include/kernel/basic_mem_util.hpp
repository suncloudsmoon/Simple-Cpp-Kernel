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