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

#ifndef LIB_SHARED_PTR
#define LIB_SHARED_PTR

#include <stddef.h>

#include <lib/zassert.hpp>

namespace zl {
	template<typename T>
	class shared_ptr {
		public:
			shared_ptr() = default;
			explicit shared_ptr(T *pointer) : ptr(pointer) {}
			shared_ptr(const shared_ptr &other) {
				if (!other.counter) { ptr = nullptr; counter = nullptr; return; }
				ptr = other.ptr;
				counter = other.counter;
				*(counter)++;
			}
			shared_ptr(shared_ptr &&other) noexcept {
				ptr = other.ptr;
				counter = other.counter;
				
				other.ptr = nullptr;
				other.counter = nullptr;
			}
			~shared_ptr() {
				if (!counter) { delete ptr; return; }
				if (!(*counter)) {
					delete ptr;
					delete counter;
				} 
				(*counter)--;
			}
			shared_ptr& operator=(const shared_ptr &other) {
				if (!other.counter) {
					return *this;
				} else if (!*(other.counter)) {
					delete ptr;
					delete counter;
				}
				ptr = other.ptr;
				counter = other.counter;
				*(counter)++;

				return *this;
			}
			shared_ptr& operator=(shared_ptr &&other) noexcept {
				delete ptr;
				delete counter;

				ptr = other.ptr;
				counter = other.counter;

				other.ptr = nullptr;
				other.counter = nullptr;

				return *this;
			}
			operator bool() const { return ptr; }
			
			T* get_ptr() const { return ptr; }
			T& operator*() const { 
				assert(get_ptr(), "[zl::shared_ptr<T>::operator*() error] -> dereferencing a null pointer!");
				return *get_ptr();
			}
			T* operator->() const {
				assert(get_ptr(), "[zl::shared_ptr<T>::operator->() error] -> pointer is null!");
				return get_ptr();
			}
		private:
			T *ptr = nullptr;
			size_t *counter = new size_t{};
	};
}

#endif /* LIB_SHARED_PTR */