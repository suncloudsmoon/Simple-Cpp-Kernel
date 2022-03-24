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

#ifndef LIB_STACK_HPP
#define LIB_STACK_HPP

#include <lib/growing_array.hpp>

namespace zl {
	template<typename T>
	class stack : public growing_array<T> {
		public:
			using growing_array<T>::growing_array;
			using growing_array<T>::operator=;

			stack(const stack &other) : growing_array<T>(other) {}
			stack(stack &&other) noexcept : growing_array<T>(other) {}

			inline T& front() {
				return *growing_array<T>::operator[](growing_array<T>::get_curr_len() - 1);
			}
			inline bool push(const T &other) {
				return growing_array<T>::add(other);
			}
			inline bool push(T &&other) {
				return growing_array<T>::add(other);
			}
			inline bool pop() {
				return growing_array<T>::remove(growing_array<T>::get_curr_len() - 1); 
			}
			inline size_t size() const {
				return growing_array<T>::get_curr_len();
			}
			inline bool empty() const {
				return !size();
			}
	};
}

#endif /* LIB_STACK_HPP */