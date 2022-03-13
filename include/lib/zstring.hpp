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

namespace zl {
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
			friend ostream& operator<<(ostream &out, const string &str);
		private:
			unexpected<blk> auto_realloc(size_t add_num);
			char *data = nullptr;
			size_t curr_index = 0;
			size_t len = 0;
	};
}

#endif /* LIB_ZSTRING_HPP */