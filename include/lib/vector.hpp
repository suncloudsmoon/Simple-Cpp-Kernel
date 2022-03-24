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

#ifndef LIB_VECTOR_HPP
#define LIB_VECTOR_HPP

#include <lib/growing_array.hpp>

namespace zl {
	template<typename T>
	class vector : public growing_array<T> {
		public:
			using growing_array<T>::growing_array;
			using growing_array<T>::operator=;
			
			/*
			 * Learned: It turns out that default, copy, and move constructors are not inherited 
			 * because they are special.
			 * https://stackoverflow.com/questions/37585945/c-inherited-copy-ctors-does-not-work
			 */
			/* 
			 * No need to make a user-defined default constructor because 
			 * growing_array<T>::growing_array(size_t) takes in a size_t as parameter
			 * with "size_t arg = 10" as the default value.
			 */ 
			vector(const vector &other) : growing_array<T>(other) {}
			vector(vector &&other) noexcept : growing_array<T>(other) {}

			inline bool push_back(const T &ele) {
				return growing_array<T>::add(ele);
			}
			inline bool push_back(T &&ele) {
				return growing_array<T>::add(ele);
			}
			inline bool remove(size_t index) {
				return growing_array<T>::remove(index);
			}
			inline bool clear() {
				return growing_array<T>::clear();
			}

			inline expected<T&> at(size_t index) const {
				assert(*this, "[zl::vector<T>::at(size_t) error] -> object not initialized properly (null)!");
				if (auto res = growing_array<T>::at(index)) {
					T *ptr = *res;
					return (!ptr) ? (expected<T&>{ *this[0], "[zl::vector<T>::at(size_t) error] -> pointer is null!" }) : (*ptr);
				} else {
					return { *this[0], "[zl::vector<T>::at(size_t) error] -> index out of bounds!", -1 }; 
				}
			}
			inline size_t size() const {
				return growing_array<T>::get_curr_len();
			}
			inline bool empty() const {
				return !size();
			}
			inline T* data() const {
				assert(growing_array<T>::get_raw_arr(), "[zl::vector<T>::data() error] -> data is null!");
				return growing_array<T>::get_raw_arr()[0];
			}
	};
}

#endif /* LIB_VECTOR_HPP */