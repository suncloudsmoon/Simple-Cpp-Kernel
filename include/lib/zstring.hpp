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

namespace zl {
	struct strdup_info {
		char *buf;
		size_t len;
		operator bool() { return buf; }
	};
	/*
	 * Length excluding null terminator
	 */
	inline constexpr size_t strlen(const char *str) {
		if (!str) return 0;
		size_t index{0};
		while (str[index++]);
		return index;
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

			string substr(size_t start, size_t end);
			char& at(size_t index);
			
			// Getters
			const char* c_str() const {
				return data;
			}
			size_t length() const {
				return len;
			}
			// TODO: do it like the std::string operator=(std::nullptr_t) = delete (C++23)
			// string& operator=(nullptr) = delete;
			string& operator=(const char *other);
			string& operator=(const string &other);
			string& operator=(string &&other);

			operator bool();
			char& operator[](size_t index);

			string& operator+=(char c);
			string& operator+=(const char *other);
			string& operator+=(const string &other);

			bool operator==(const char *other) const;
			bool operator==(const string &other) const;
			bool operator!=(const char *other) const;
			bool operator!=(const string &other) const;

		private:
			unexpected<os::blk> auto_realloc(size_t add_num);
			bool is_usable;
			char *data;
			size_t curr_index;
			size_t len;
			char null_dat = 0;
	};

	expected<string> itoa(int num);
}

#endif /* LIB_ZSTRING_HPP */