#include <lib/zmem.hpp>

namespace zl {
	template<>
	bool memcpy(void *dest, const void *src, size_t bytes) {
		if (!dest || !src || !bytes)
			return false;

		/*
		 * Very interesting... it is possible to cast void* to long* to do the 
		 * copy operation (CPU accessing a data type that is at least long width).
		 */
		int_fast32_t *dest_ptr = reinterpret_cast<int_fast32_t*>(dest);
		const int_fast32_t *src_ptr = reinterpret_cast<const int_fast32_t*>(src);
		
		for (size_t i = 0; i < bytes / sizeof(int_fast32_t); i++) 
			dest_ptr[i] = src_ptr[i];
		
		return true;
	}
}