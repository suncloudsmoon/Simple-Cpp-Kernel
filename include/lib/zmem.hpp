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

#ifndef LIB_ZMEM_HPP
#define LIB_ZMEM_HPP

#include <stddef.h>
#include <stdint.h>
#include <lib/zutil.hpp>
#include <kernel/basic_mem_util.hpp>

// Other parts of zmem in different files
#include <lib/unique_ptr.hpp>

// Defining NULL as prescribed in the C standard
#ifdef NULL
#undef NULL
#endif
#define NULL 0

namespace zl {
	using blk = os::mem::blk;
	
	template<typename T>
	T&& move(T &ref) {
		return static_cast<T&&>(ref);
	}

	/*
	Can have overlapping memory regions
	*/
	template<typename T>
	inline bool memcpy(T *dest, const T *src, size_t src_num_cpy) {
		if (!dest || !src || !src_num_cpy)
			return false;
		
		for (size_t i = 0; i < src_num_cpy; i++) 
			dest[i] = src[i];
		
		return true;
	}

	template<>
	inline bool memcpy(void *dest, const void *src, size_t src_num_cpy) {
		if (!dest || !src || !src_num_cpy)
			return false;

		// Interesting... they cast void* to long* to do the copy operation (CPU accessing a data type that is at least long width)
		int32_t *dest_lon = reinterpret_cast<int32_t*>(dest);
		const int32_t *src_lon = reinterpret_cast<const int32_t*>(src);
		
		for (size_t i = 0; i < src_num_cpy; i++) 
			dest_lon[i] = src_lon[i];
		
		return true;
	}
	
	inline expected<blk> malloc(size_t size) {
		return os::mem::alloc(size);
	}
	inline expected<blk> realloc(void *src, size_t size) {
		return os::mem::realloc(src, size);
	}
	inline bool free(void *ptr) {
		return os::mem::free(ptr);
	}
}

// C++ global new/delete operators
inline void* operator new(size_t size) {
	return (*zl::malloc(size)).ptr;
}
inline void* operator new[](size_t size) {
	return (*zl::malloc(size)).ptr;
}
inline void operator delete(void *ptr) {
	zl::free(ptr);
}
inline void operator delete(void *ptr, size_t size) {
	zl::free(ptr);
}
inline void operator delete[](void *ptr) {
	zl::free(ptr);
}

// Placement new
inline void* operator new(size_t size, void *ptr) {
	return ptr; 
}
inline void* operator new[](size_t size, void *ptr) { 
	return ptr; 
}
inline void operator delete(void *ptr1, void *ptr2) {}
inline void operator delete[](void *ptr1, void *ptr2) {}

#endif /* LIB_ZMEM_HPP */