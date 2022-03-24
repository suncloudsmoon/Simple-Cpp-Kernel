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

#include <lib/zmem.hpp>

namespace zl {
	struct strdup_info {
		constexpr strdup_info(char *buf_str = nullptr, size_t string_len = 0, size_t buffer_length = 0) : 
			buf(buf_str), str_len(string_len), buf_len(buffer_length) {}
		constexpr operator bool() { return buf; }
		char *buf;
		size_t str_len;
		size_t buf_len;
	};
	strdup_info strdup(const char *src);
	/*
	 * Length excluding null terminator
	 */
	inline constexpr size_t strlen(const char *str) {
		if (!str) return 0;
		size_t index = 0;
		while (str[index++]);
		return index - 1;
	}
	inline constexpr bool strncat(char *dest, const char *src, size_t len) {
		if (!dest || !src || !len) 
			return false;
		size_t dest_len = strlen(dest);
		if (!memcpy(dest + dest_len, src, len))
			return false;
		dest[dest_len + len] = '\0'; 
		return true;	
	}
	inline constexpr bool strncpy(char *dest, const char *src, size_t len) {
		if (!dest || !src || !len)
			return false;
		if (!memcpy(dest, src, len))
			return false;
		dest[len] = '\0';
		return true;	
	}
	inline constexpr bool strequal(const char *str1, const char *str2) {
		if (!str1 || !str2)
			return false;

		size_t index = 0;
		char a, b;
		while ((a = str1[index]) && (b = str2[index])) {
			if (str1[index] != str2[index])
				return false;
			index++;
		}
		return (str1[index] == '\0') && (str2[index] == '\0');
	}
}

#endif /* LIB_ZSTRINGUTIL_HPP */