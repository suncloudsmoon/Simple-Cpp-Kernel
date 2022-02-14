#ifndef LIB_GROWING_ARRAY
#define LIB_GROWING_ARRAY

#include <lib/zmem.hpp>
#include <lib/zutil.hpp>

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
				return iterator(arr + curr_index);
			}
		public:
			growing_array(size_t length = 10) : arr(nullptr), curr_index(0), len(0) {
				if (auto res = malloc(length * sizeof(T*))) [[likely]] {
					arr = static_cast<T**>((*res).ptr);
					len = length;
				}
			}
			growing_array(const growing_array &other) : curr_index(0), len(0) {
				if (auto res = allocate_at_least(other.len)) [[likely]] {
					if (!memcpy(arr, other.arr, other.len)) [[unlikely]] {
						free(arr);
						arr = nullptr;
						return;
					}
					curr_index = other.curr_index;
					len = other.len;
				}
			}
			growing_array(growing_array &&other) {
				arr = other.arr;
				curr_index = other.curr_index;
				len = other.len;

				other.arr = nullptr;
				other.curr_index = 0;
				other.len = 0;
			}
			~growing_array() {
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
					curr_index = other.curr_index;
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
				curr_index = other.curr_index;
				len = other.len;

				other.arr = nullptr;
				other.curr_index = 0;
				other.len = 0;
			}
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
				return arr[index];
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
				if (!auto_realloc(1)) [[unlikely]]
					return false;
				arr[curr_index++] = item;
				return true;
			}
			bool remove(size_t index) {
				if (index >= len) [[unlikely]]
					return false;
				delete arr[index];
				if (!memcpy(arr[index], arr[index + 1], len - index)) [[unlikely]]
					return false;
				return true;
			}
			void clear() {
				for (size_t i = 0; i < len; i++)
					delete arr[i];
				curr_index = 0;				
			}

			T** get_raw_arr() {
				return arr;
			}
			size_t get_curr_index() {
				return curr_index;
			}
			size_t get_len() {
				return len;
			}
		protected:
			void set_raw_arr(T **other) {
				arr = other;
			}
			void set_curr_index(size_t new_index) {
				curr_index = new_index;
			}
			void set_len(size_t new_len) {
				len = new_len;
			}
		private:
			unexpected<os::blk> auto_realloc(size_t add_num) {
				if (curr_index + add_num + 1 >= len) {
					if (auto res = realloc(arr, curr_index + add_num + 1)) [[likely]] {
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
			size_t curr_index;
			size_t len;
	};
}

#endif /* LIB_GROWING_ARRAY */