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

#include <lib/zmem.hpp>

namespace zl {
	namespace {
		template<typename T>
		bool cpy_arr(void *dest, const void *src, size_t bytes) {
			if (!dest || !src || !bytes)
				return false;

			/*
			* Very interesting... it is possible to cast void* to long* to do the 
			* copy operation (CPU accessing a data type that is at least long width).
			*/
			T *dest_ptr = reinterpret_cast<T*>(dest);
			const T *src_ptr = reinterpret_cast<const T*>(src);
			
			for (size_t i = 0; i < bytes / sizeof(int_fast32_t); i++) 
				dest_ptr[i] = src_ptr[i];
			
			return true;
		}
	}
	
	bool memcpy(void *dest, const void *src, size_t bytes) {
		if (bytes < sizeof(int_fast32_t))
			return cpy_arr<unsigned char>(dest, src, bytes);
		else
			return cpy_arr<int_fast32_t>(dest, src, bytes);
	}
}