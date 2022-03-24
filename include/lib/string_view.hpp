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

#ifndef LIB_STRING_VIEW_HPP
#define LIB_STRING_VIEW_HPP

#include <stddef.h>

#include <lib/zstringutil.hpp>
#include <lib/zutil.hpp>
#include <lib/zmem.hpp>

namespace zl {
	// const char pointer wrapper (constexpr)
	class string_view {
		public:
			class const_iterator {
				public:
					constexpr const_iterator(const char *pointer) : ptr(const_cast<char*>(pointer)) {}
					constexpr operator bool() const { return ptr; }

					constexpr const_iterator operator++() { ptr++; return *this; }
					constexpr char operator*() const { return *ptr; }

					constexpr bool operator==(const const_iterator &other) const { return ptr == other.ptr; }
					constexpr bool operator!=(const const_iterator &other) const { return ptr != other.ptr; }
				private:
					char *ptr;
			};
		public:
			using iterator = const_iterator;

			constexpr string_view(const char *message = nullptr) : str(message), len(strlen(str)) {}
			constexpr string_view(const char *message, size_t length) : str(message), len(length) {}
			constexpr operator bool() const { return data(); }

			/* No bounds checking */
			constexpr char at(size_t index) const { return str[index]; }

			constexpr bool equals(const string_view &other) const { return strequal(str, other.str); }
			
			constexpr size_t length() const { return len; }
			constexpr const char* data() const { return str; }
			
			constexpr const char* operator*() const { return data(); }
			constexpr char operator[](size_t index) const { return at(index); }

			constexpr bool operator==(const string_view &other) const { return equals(other); }
			constexpr bool operator!=(const string_view &other) const { return !equals(other); }

			constexpr iterator begin() const { return iterator(str); }
			constexpr iterator end() const { return iterator(str + len); }
		private:
			const char *str;
			size_t len;
	};
}

#endif /* LIB_STRING_VIEW_HPP */