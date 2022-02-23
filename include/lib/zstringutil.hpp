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

#ifndef LIB_ZSTRINGUTIL_HPP
#define LIB_ZSTRINGUTIL_HPP

#include <lib/zstring.hpp>

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

	template<typename T>
	inline expected<zl::string> itoa(T num) {
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
			return {nullptr, "[zl::itoa() error] -> zl::stack::stack() constructor error!"};
		}
		return {nullptr, "[zl::itoa() error] -> unknown error!"};
	}
}

#endif /* LIB_ZSTRINGUTIL_HPP */