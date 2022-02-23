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

#ifndef LIB_GROWING_ARRAY
#define LIB_GROWING_ARRAY

#include <lib/zmem.hpp>
#include <lib/zutil.hpp>
#include <lib/zassert.hpp>

namespace zl {
	template<typename T>
	class growing_array {
		public:
			class iterator {
				public:
					iterator(T **ptr) : pos(ptr) {}
					bool operator!=(const iterator &other) const {
						return pos != other.pos;
					}
					iterator operator++() {
						pos++;
						return *this;
					}
					T& operator*() {
						return *(*pos);
					}
				private:
					T **pos;
			};
			class const_iterator {
				public:
					const_iterator(T **ptr) : pos(ptr) {}
					bool operator!=(const const_iterator &other) const {
						return pos != other.pos;
					}
					iterator operator++() {
						pos++;
						return *this;
					}
					const T& operator*() {
						return *(*pos);
					}
				private:
					T **pos;
			};
			iterator begin() {
				return iterator(arr);
			}
			iterator end() {
				return iterator(arr + curr_len);
			}
		public:
			growing_array(size_t length = 10) : arr(nullptr), curr_len(0), len(0) {
				if (auto res = malloc(length * sizeof(T*))) [[likely]] {
					arr = static_cast<T**>((*res).ptr);
					len = length;
				}
			}
			growing_array(const growing_array &other) : curr_len(0), len(0) {
				if (auto res = allocate_at_least(other.len)) [[likely]] {
					if (!memcpy(arr, other.arr, other.len)) [[unlikely]] {
						free(arr);
						arr = nullptr;
						return;
					}
					curr_len = other.curr_len;
					len = other.len;
				}
			}
			growing_array(growing_array &&other) {
				arr = other.arr;
				curr_len = other.curr_len;
				len = other.len;

				other.arr = nullptr;
				other.curr_len = 0;
				other.len = 0;
			}
			virtual ~growing_array() {
				// Since we are utilizing a move constructor, arr being nullptr should be valid
				if (arr) [[likely]] {
					clear();
					free(arr);
				}
			}
			/* Checking if the array allocated through constructor is valid */
			operator bool() { return arr; }
			bool operator=(const growing_array &other) {
				if (auto res = allocate_at_least(other.len)) [[likely]] {
					if (arr) [[likely]] {
						clear();
						free(arr);
					}
					if (!memcpy(arr, other.arr, other.len)) [[unlikely]] {
						free(arr);
						arr = nullptr;
						return false;
					}
					curr_len = other.curr_len;
					len = other.len;
					return true;
				} 
				return false;
			}
			void operator=(growing_array &&other) {
				if (arr) [[likely]] {
					clear();
					free(arr);
				}
				arr = other.arr;
				curr_len = other.curr_len;
				len = other.len;

				other.arr = nullptr;
				other.curr_len = 0;
				other.len = 0;
			}
		protected:	
			unexpected<os::blk> allocate_at_least(size_t length) {
				if (auto res = malloc(length * sizeof(T*))) [[likely]] {
					arr = static_cast<T**>((*res).ptr);
					len = (*res).len;
					return {};
				} else {
					return res;
				}
			}
			expected<T*> at(size_t index) {
				if (index >= len) [[unlikely]]
					return {nullptr, "at() error -> index is greater than length of array!", -1};
				return arr[index];
			}
			T* operator[](size_t index) {
				zl::assert(index < len, "[growing_array::operator[] error] -> index out of bounds!");
				return arr[index];
			}
			bool add(size_t index, const T &item) {
				T *new_obj = new T(item);
				if (!add(index, new_obj)) {
					delete new_obj;
					return false;
				}
				return true;
			}
			bool add(size_t index, T &&item) {
				T *new_obj = new T(item);
				if (!add(index, new_obj)) {
					delete new_obj;
					return false;
				}
				return true;
			}

			bool add(size_t index, T *item) {
				if (!auto_realloc(1))
					return false;
				if (!memcpy(arr[index + 1], arr[index], curr_len))
					return false;
				arr[index] = item;
				curr_len++;
				return true;		
			}

			bool add(const T &item) {
				T *new_item = new T(item);
				if (!add(new_item)) [[unlikely]] {
					delete new_item;
					return false;
				}
				return true;
			}
			bool add(T &&item) {
				T *new_item = new T(item);
				if (!add(new_item)) [[unlikely]] {
					delete new_item;
					return false;
				}
				return true;
			}
			bool add(T *item) {
				zl::assert(item != nullptr, "[growing_array::add(T*) error] -> item pointer arg is null!");
				if (!auto_realloc(1)) [[unlikely]]
					return false;
				arr[curr_len++] = item;
				return true;
			}
			bool remove(size_t index) {
				if (index >= len) [[unlikely]]
					return false;
				delete arr[index];
				curr_len--;
				if (!memcpy(arr[index], arr[index + 1], len - index)) [[unlikely]]
					return false;
				return true;
			}
			void clear() {
				zl::assert(arr != nullptr, "[growing_array::clear() error] -> string's char array is null!");
				for (size_t i = 0; i < len; i++)
					delete arr[i];
				curr_len = 0;				
			}

			T** get_raw_arr() {
				return arr;
			}
			size_t get_curr_len() const {
				return curr_len;
			}
			size_t get_len() const {
				return len;
			}
			void set_raw_arr(T **other) {
				arr = other;
			}
			void set_curr_len(size_t new_len) {
				curr_len = new_len;
			}
			void set_len(size_t new_len) {
				len = new_len;
			}
		private:
			unexpected<os::blk> auto_realloc(size_t add_num) {
				if (curr_len + add_num + 1 >= len) {
					if (auto res = realloc(arr, curr_len + add_num + 1)) [[likely]] {
						arr = static_cast<T**>((*res).ptr);
						len = (*res).len;
						return {};					
					} else {
						return res;
					}
				}
				return {};
			}
			T **arr;
			size_t curr_len;
			size_t len;
	};
}

#endif /* LIB_GROWING_ARRAY */