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

namespace zl {
	struct strdup_info {
		strdup_info(char *buf_str = nullptr, size_t string_len = 0, size_t buffer_length = 0) : 
			buf(buf_str), str_len(string_len), buf_len(buffer_length) {}
		char *buf;
		size_t str_len;
		size_t buf_len;
		operator bool() { return buf; }
	};
	/*
	 * Length excluding null terminator
	 */
	inline constexpr size_t strlen(const char *str) {
		if (!str) return 0;
		size_t index{0};
		while (str[index++]);
		return index - 1;
	}
	strdup_info strdup(const char *src);
	bool strncat(char *dest, const char *src, size_t len);
	bool strncpy(char *dest, const char *src, size_t len);
	bool strequal(const char *str1, const char *str2);

	class string {
		public:
			string(size_t init_size = 10);
			string(const char *str_ptr);
			string(const string &other);
			string(string &&other);
			~string();

			bool append(char c);
			bool append(const char *other);
			bool append(const string &other);

			bool equals(const char *other) const;
			bool equals(const string &other) const;

			bool set(const char *other);
			bool set(const string &other);
			bool set(string &&other);

			string substr(size_t start, size_t end);
			expected<char> at(size_t index);
			
			// Getters
			const char* c_str() const {
				return data;
			}
			size_t length() const {
				return curr_index;
			}

			operator bool() { return data; }
			inline char& operator[](size_t index) {
				zl::assert(data != nullptr, "[zl::string::operator[] error] -> data is null!");
				zl::assert(index < len, "[zl::string::operator[] error] -> index out of bounds!");
				return data[index];
			}

			inline string& operator=(const char *other) {
				set(other);
				return *this;
			}
			inline string& operator=(const string &other) {
				set(other);
				return *this;
			}
			inline string& operator=(string &&other) {
				set(other);
				return *this;
			}

			inline string& operator+=(char c) {
				append(c);
				return *this;
			}
			inline string& operator+=(const char *other) {
				append(other);
				return *this;
			}
			inline string& operator+=(const string &other) {
				append(other);
				return *this;
			}

			inline bool operator==(const char *other) const {
				return equals(other);
			}
			inline bool operator==(const string &other) const {
				return equals(other);
			}
			inline bool operator!=(const char *other) const {
				return !equals(other);
			}
			inline bool operator!=(const string &other) const {
				return !equals(other);
			}
		private:
			unexpected<os::blk> auto_realloc(size_t add_num);
			char *data;
			size_t curr_index;
			size_t len;
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
				char fr = st.front();
				res += fr;
				st.pop();
			}
			return res;
		} else {
			return {nullptr, "[zl::itoa() error] -> zl::stack::stack() constructor error!"};
		}
		return {nullptr, "[zl::itoa() error] -> unknown error!"};
	}
}

#endif /* LIB_ZSTRING_HPP */