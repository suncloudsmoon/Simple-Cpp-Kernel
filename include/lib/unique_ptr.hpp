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

#ifndef LIB_UNIQUE_PTR
#define LIB_UNIQUE_PTR

namespace zl {
	template<typename T>
	class unique_ptr {
		public:
			unique_ptr() = default;
			explicit unique_ptr(T* init) : ptr(init) {}
			unique_ptr(const unique_ptr &other) = delete;
			unique_ptr(unique_ptr &&other) {
				ptr = other.ptr;
				other.ptr = nullptr;
			}
			~unique_ptr() { delete ptr; }
			unique_ptr& operator=(const unique_ptr &other) = delete;
			unique_ptr& operator=(unique_ptr &&other) {
				delete ptr;
				ptr = other.ptr;
				other.ptr = nullptr;
				return *this;
			}
			operator bool() { return ptr; }

			T* get_ptr() { return ptr; }
			T* operator*() { return get_ptr(); }
			T* operator->() { return get_ptr(); }
		private:
			T *ptr = nullptr;
	};
}

#endif /* LIB_UNIQUE_PTR */