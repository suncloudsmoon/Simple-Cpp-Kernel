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

#include <lib/zstring.hpp>
#include <lib/zassert.hpp>

namespace zl {
	strdup_info strdup(const char *src) {
		if (!src)
			return {nullptr, 0};
		size_t src_len = strlen(src);
		auto res = malloc(src_len);
		if (auto res = malloc(src_len)) {
			if (!memcpy(static_cast<char*>((*res).ptr), src, src_len)) {
				free((*res).ptr);
				return {nullptr, 0};
			}
			return {static_cast<char*>((*res).ptr), (*res).len};
		} else {
			return {nullptr, 0};
		}	
	}

	// REMINDER: Need to null terminate dest string
	bool strncat(char *dest, const char *src, size_t len) {
		if (!dest || !src || !len) 
			return false;
		size_t dest_len = strlen(dest);
		if (!memcpy(dest + dest_len, src, len))
			return false;
		dest[dest_len + len] = '\0'; 
		return true;	
	}

	bool strncpy(char *dest, const char *src, size_t len) {
		if (!dest || !src || !len)
			return false;
		if (!memcpy(dest, src, len))
			return false;
		dest[len] = '\0';
		return true;	
	}

	bool strequal(const char *str1, const char *str2) {
		if (!str1 || !str2)
			return false;
		size_t index{0};
		char a, b;
		while ((a = str1[index]) && (b = str2[index])) {
			if (a != b)
				return false;
			index++;
		}
		return str1[index] == '\0';
	}

	string::string(size_t init_size) : is_usable(true), data(nullptr), curr_index(0), len(0) {
		if (!init_size)
			return;
		if (auto res = malloc(init_size)) {
			char *data = static_cast<char*>((*res).ptr);
			data[init_size - 1] = '\0';
			len = (*res).len;
		} else {
			is_usable = false;
		}
	}
	string::string(const char *src) : is_usable(true), data(nullptr), curr_index(0), len(0) {
		if (auto info = strdup(src)) {
			data = info.buf;
			len = info.len;
		}
	}
	string::string(const string &other) : is_usable(true), data(nullptr), curr_index(0), len(0) {
		if (auto info = strdup(other.c_str())) {
			data = info.buf;
			len = info.len;
		}
	}
	string::string(string &&other) {
		is_usable = other.is_usable;
		data = other.data;
		curr_index = other.curr_index;
		len = other.len;

		other.is_usable = false;
		other.data = nullptr;
		other.curr_index = 0;
		other.len = 0;
	}
	string::~string() {
		free(data);
	}

	string string::substr(size_t start, size_t end) {
		string new_str;
		for (size_t i = start; i < end; i++)
			new_str += data[i];
		return new_str;
	}

	char& string::at(size_t index) {
		if (index >= len) return null_dat;
		return data[index];
	}

	string& string::operator=(const char *other) {
		if (!other)
			return *this;
		size_t other_len = strlen(other);
		if (!auto_realloc(other_len + 1))
			return *this;
		if (strncpy(data, other, other_len))
			curr_index = other_len + 1;
		return *this;	
	}
	string& string::operator=(const string &other) {
		if (!other.data || !other.is_usable)
			return *this;
		if (!auto_realloc(other.len))
			return *this;
		if (strncpy(data, other.data, other.len))
			curr_index = len;
		return *this;	
	}
	string& string::operator=(string &&other) {
		free(data);
		is_usable = other.is_usable;
		data = other.data;
		curr_index = other.curr_index;
		len = other.len;

		other.is_usable = false;
		other.data = nullptr;
		other.curr_index = 0;
		other.len = 0;

		return *this;
	}

	string::operator bool() { return is_usable; }

	char& string::operator[](size_t index) {
		zl::assert(index < len, "[string::operator[] error] -> index out of bounds!");
		return data[index];
	}

	string& string::operator+=(char c) {
		if (!auto_realloc(curr_index + 2))
			return *this;
		data[curr_index++] = c;
		data[curr_index++] = '\0';	
		return *this;
	}

	string& string::operator+=(const char *other) {
		if (!other)
			return *this;
		size_t other_len = strlen(other);
		if (!auto_realloc(other_len + 1))
			return *this;
		if (strncat(data, other, other_len))
			curr_index += other_len;
		return *this;
	}
	string& string::operator+=(const string &other) {
		if (!auto_realloc(other.curr_index + 1))
			return *this;
		if (strncat(data, other.c_str(), other.curr_index + 1))
			curr_index += other.curr_index + 1;
		return *this;
	}

	bool string::operator==(const char *other) const {
		return strequal(c_str(), other);
	}
	bool string::operator==(const string &other) const {
		return strequal(c_str(), other.c_str());
	}
	bool string::operator!=(const char *other) const {
		return !strequal(c_str(), other);
	}
	bool string::operator!=(const string &other) const {
		return !strequal(c_str(), other.c_str());
	}

	// Private member(s) of the class
	unexpected<os::blk> string::auto_realloc(size_t add_num) {
		if (curr_index + add_num + 1 >= len) {
			if (auto res = malloc(curr_index + add_num + 1)) {
				data = static_cast<char*>((*res).ptr);
				len = (*res).len;
				return nullptr;
			} else {
				return res;
			}
		}
		return nullptr;
	}
}
