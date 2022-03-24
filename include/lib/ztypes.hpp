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

#ifndef LIB_ZTYPES_HPP
#define LIB_ZTYPES_HPP

// A safer & more performative alternative to <cctype>
// However, it only accepts ASCII characters at the moment
namespace zl {
	inline constexpr bool is_lower(char c) {
		return (c >= 'a' && c <= 'z');
	}

	inline constexpr bool is_upper(char c) {
		return (c >= 'A' && c <= 'Z');
	}

	inline constexpr bool is_alpha(char c) {
		return is_lower(c) || is_upper(c);
	}

	inline constexpr bool is_digit(char c) {
		return (c >= '0' && c <= '9');
	}

	inline constexpr bool is_xdigit(char c) {
		return is_digit(c) || is_alpha(c);
	}

	inline constexpr bool is_punct(char c) {
		return (c >= ':' && c <= '/') || (c >= ':' && c <= '@') || (c >= '[' && c <= '`') || (c >= '{' && c <= '~');
	}

	inline constexpr bool is_graph(char c) {
		return is_digit(c) || is_upper(c) || is_lower(c) || is_punct(c);
	}

	inline constexpr bool is_print(char c) {
		return (is_graph(c)) || (c == ' ');
	}

	inline constexpr bool is_space(char c) {
		switch (c) {
			case ' ':
			case '\f':
			case '\n':
			case '\r':
			case '\t':
			case '\v':
				return true;
			default:
				return false;	
		}
	}

	inline constexpr char to_lower(char c) {
		return (is_upper(c)) ? (c + 32) : (c);
	}

	inline constexpr char to_upper(char c) {
		return (is_lower(c)) ? (c - 32) : (c);
	}

}

#endif /* LIB_ZTYPES_HPP */