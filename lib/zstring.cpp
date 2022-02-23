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
#include <lib/zstringutil.hpp>

namespace zl {
	string::string(size_t init_size) : data(nullptr), curr_index(0), len(0) {
		if (!init_size)
			return;
		if (auto res = malloc(init_size)) {
			data = static_cast<char*>((*res).ptr);
			data[0] = '\0';
			len = (*res).len;
		} else {
			free((*res).ptr);
			zl::assert(false, "[zl::string::string error] -> unable to allocate memory for string!");
		}
	}
	string::string(const char *src) : data(nullptr), curr_index(0), len(0) {
		if (auto info = strdup(src)) {
			data = info.buf;
			curr_index = info.str_len;
			len = info.buf_len;
		}
	}
	string::string(const string &other) : data(nullptr), curr_index(0), len(0) {
		if (auto info = strdup(other.c_str())) {
			data = info.buf;
			len = info.buf_len;
		}
	}
	string::string(string &&other) {
		data = other.data;
		curr_index = other.curr_index;
		len = other.len;

		other.data = nullptr;
		other.curr_index = 0;
		other.len = 0;
	}
	string::~string() {
		free(data);
	}

	bool string::append(char c) {
		if (!auto_realloc(curr_index + 2))
			return false;
		data[curr_index++] = c;
		data[curr_index] = '\0';
		return true;
	}
	bool string::append(const char *other) {
		if (!other)
			return false;
		size_t other_len = strlen(other);
		if (!auto_realloc(other_len + 1))
			return false;
		if (strncat(data, other, other_len))
			curr_index += other_len;
		return true;
	}
	bool string::append(const string &other) {
		if (!other.c_str())
			return false;
		if (!auto_realloc(other.curr_index + 1))
			return false;
		if (strncat(data, other.c_str(), other.curr_index))
			curr_index += other.curr_index;
		return true;
	}

	bool string::equals(const char *other) const {
		return strequal(c_str(), other);
	}
	bool string::equals(const string &other) const {
		return strequal(c_str(), other.c_str());
	}

	bool string::set(const char *other) {
		if (!other)
			return false;
		size_t other_len = strlen(other);
		if (!auto_realloc(other_len + 1))
			return false;
		if (strncpy(data, other, other_len))
			curr_index = other_len;
		return true;
	}
	bool string::set(const string &other) {
		if (!other.data)
			return false;
		if (!auto_realloc(other.len))
			return false;
		if (strncpy(data, other.data, other.len))
			curr_index = other.curr_index;
		return true;
	}
	bool string::set(string &&other) {
		free(data);
		data = other.data;
		curr_index = other.curr_index;
		len = other.len;

		other.data = nullptr;
		other.curr_index = 0;
		other.len = 0;
		return true;
	}

	string string::substr(size_t start, size_t end) {
		string new_str;
		for (size_t i = start; i < end; i++)
			new_str += data[i];
		return new_str;
	}

	expected<char> string::at(size_t index) {
		if (index >= len) return {data[0], "[zl::string::at() error] -> index out of bounds!", -1};
		return data[index];
	}

	ostream& operator<<(ostream &out, const string &str) {
		return out << str.c_str();
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
