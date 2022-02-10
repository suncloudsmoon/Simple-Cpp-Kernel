#ifndef LIB_ZMEM_HPP
#define LIB_ZMEM_HPP

#include <kernel/basic_mem_util.hpp>
#include <stddef.h>

// Other parts of zmem in different files
#include <lib/unique_ptr.hpp>

namespace zl {

template<typename T>
T&& move(T &ref) {
	return static_cast<T&&>(ref);
}

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
	long *dest_lon = reinterpret_cast<long*>(dest);
	const long *src_lon = reinterpret_cast<const long*>(src);
	
	for (size_t i = 0; i < src_num_cpy; i++) 
		dest_lon[i] = src_lon[i];
	
	return true;
}

void* malloc(size_t size);
void* realloc(void *ptr, size_t size, size_t *aligned_size_res);
bool free(void *ptr);
}

// C++ global new/delete operators
void* operator new(size_t size);
void* operator new[](size_t size);
void operator delete(void *ptr);
void operator delete[](void *ptr);

#endif /* LIB_ZMEM_HPP */