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

// Other parts of zmem in different files
#include <lib/unique_ptr.hpp>
#include <lib/shared_ptr.hpp>

#include <kernel/basic_mem_util.hpp>

// Defining NULL as prescribed in the C standard
#ifdef NULL
#undef NULL
#endif
#define NULL 0

namespace zl {
	using blk = os::mem::blk;
	
	template<typename T>
	inline T&& move(T &ref) {
		return static_cast<T&&>(ref);
	}

	/*
	 * Can have overlapping memory regions
	 */
	template<typename T>
	inline constexpr bool memcpy(T *dest, const T *src, size_t bytes) {
		if (!dest || !src || !bytes) return false;
		for (size_t i = 0; i < bytes / sizeof(T); i++) { dest[i] = src[i]; }
		return true;
	}
	bool memcpy(void *dest, const void *src, size_t bytes);

	template<typename T>
	inline constexpr bool memset(T *ptr, T val, size_t bytes) {
		if (!ptr || !bytes) return false;
		for (size_t i = 0; i < bytes / sizeof(T); i++) { ptr[i] = val; }
		return true;
	}
	namespace {
		template<typename T, typename CopyType>
		inline constexpr bool __set_arr__(void *ptr, T val, size_t bytes) {
			if (!ptr || !bytes) return false;
			CopyType *type_ptr = reinterpret_cast<CopyType*>(ptr);
			for (size_t i = 0; i < bytes / sizeof(T); i++) { type_ptr[i] = val; }
			return true;
		}
	}
	template<typename T>
	inline constexpr bool memset(void *ptr, T val, size_t bytes) {
		return (bytes >= sizeof(T)) ? __set_arr__<T>(ptr, val, bytes) : false;
	}

	template<typename T>
	inline constexpr bool memcmp(const T *src1, const T *src2, size_t bytes) {
		if (!src1 || !src2 || !bytes) return false;
		for (size_t i = 0; i < bytes / sizeof(T); i++) { if (src1[i] != src2[i]) return false; }
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