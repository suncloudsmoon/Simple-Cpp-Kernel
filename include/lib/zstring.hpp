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

#ifndef LIB_ZSTRING_HPP
#define LIB_ZSTRING_HPP

#include <stddef.h>

#include <lib/zutil.hpp>
#include <lib/zmem.hpp>
#include <lib/stack.hpp>
#include <lib/zio.hpp>
#include <lib/zassert.hpp>
#include <lib/string_view.hpp>
#include <lib/vector.hpp>

namespace zl {
	class string {
		public:
			static constexpr size_t npos = -1;
		public:
			class iterator {
				public:
					iterator(char *pointer) : ptr(pointer) {}
					operator bool() const { return ptr; }

					iterator operator++() { ptr++; return *this; }
					char& operator*() const { return *ptr; }

					bool operator==(const iterator &other) const { return ptr == other.ptr; }
					bool operator!=(const iterator &other) const { return ptr != other.ptr; }
				private:
					char *ptr;
			};
			class const_iterator {
				public:
					const_iterator(const char *pointer) : ptr(const_cast<char*>(pointer)) {}
					operator bool() const { return ptr; }

					const_iterator operator++() { ptr++; return *this; }
					char operator*() const { return *ptr; }

					bool operator==(const const_iterator &other) const { return ptr == other.ptr; }
					bool operator!=(const const_iterator &other) const { return ptr != other.ptr; }
				private:
					char *ptr;
			};
		public:
			string(size_t init_size = 10);
			string(const char *str_ptr);
			string(const string &other);
			string(string &&other) noexcept;
			~string();
			operator bool() const { return data; }

			bool append(char c);
			bool append(const char *other);
			bool append(const string &other);

			bool equals(const char *other) const;
			bool equals(const string &other) const;

			bool set(const char *other);
			bool set(const string &other);
			bool set(string &&other);
			
			string substr(size_t start, size_t end = npos);
			vector<string> split(string_view arg);

			size_t find(string_view arg, size_t begin = npos) const;
			expected<char&> at(size_t index) const;
			
			// Getters
			const char* c_str() const {
				return data;
			}
			size_t length() const {
				return curr_index;
			}
			
			operator string_view() { return string_view{ c_str(), length() }; }

			char& operator[](size_t index) {
				zl::assert(data != nullptr, "[zl::string::operator[](size_t) error] -> data is null!");
				zl::assert(index < len, "[zl::string::operator[](size_t) error] -> index out of bounds!");
				return data[index];
			}

			string& operator=(const char *other) {
				set(other);
				return *this;
			}
			string& operator=(const string &other) {
				set(other);
				return *this;
			}
			string& operator=(string &&other) noexcept {
				set(other);
				return *this;
			}

			string& operator+=(char c) {
				append(c);
				return *this;
			}
			string& operator+=(const char *other) {
				append(other);
				return *this;
			}
			string& operator+=(const string &other) {
				append(other);
				return *this;
			}

			bool operator==(const char *other) const {
				return equals(other);
			}
			bool operator==(const string &other) const {
				return equals(other);
			}
			bool operator!=(const char *other) const {
				return !equals(other);
			}
			bool operator!=(const string &other) const {
				return !equals(other);
			}

			iterator begin() const {
				assert(data, "[zl::string::begin() error] -> string is null!");
				return iterator(data);
			}
			iterator end() const {
				assert(data, "[zl::string::end() error] -> string is null!");
				return iterator(data + len);
			}

			const_iterator cbegin() const {
				assert(data, "[zl::string::cbegin() error] -> string is null!");
				return const_iterator(data);
			}
			const_iterator cend() const {
				assert(data, "[zl::string::cend() error] -> string is null!");
				return const_iterator(data + len);
			}

			friend ostream& operator<<(ostream &out, const string &str);
		protected:
			unexpected<blk> auto_realloc(size_t add_num);
		private:
			char *data = nullptr;
			size_t curr_index = 0;
			size_t len = 0;
	};

	template<typename T>
	inline expected<string> itoa(T num) {
		stack<char> st;
		if (st) {
			auto num_iter = num;
			while (num_iter) {
				char last = (num_iter % 10) + '0';
				st.push(last);
				num_iter /= 10;
			}
			if (num < 0)
				st.push('-');
			
			string res;
			while (!st.empty()) {
				char c = st.front();
				res += c;
				st.pop();
			}
			return res;
		} else {
			return {{}, "[zl::itoa() error] -> zl::stack::stack() constructor error!", -1};
		}
		return {{}, "[zl::itoa() error] -> unknown error!", -2};
	}
}

#endif /* LIB_ZSTRING_HPP */