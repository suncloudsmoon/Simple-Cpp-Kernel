#include <lib/zstring.hpp>
#include <lib/zmem.hpp>

namespace zl {
	strdup_info strdup(const char *src) {
		if (!src)
			return {nullptr, 0};
		size_t src_len = strlen(src);
		char *new_str = static_cast<char*>(malloc(src_len));
		if (!new_str)
			return {nullptr, 0};
		if (!memcpy(new_str, src, src_len)) {
			free(new_str);
			return {nullptr, 0};
		}
		return {new_str, src_len};
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

	string::string() : is_usable(true), data(nullptr), curr_index(0), len(0) {
		constexpr size_t INIT_SIZE{10};
		data = static_cast<char*>(malloc(INIT_SIZE));
		if (!data) {
			is_usable = false;
			return;
		}
		data[INIT_SIZE - 1] = '\0';
		curr_index = 0;
		len = INIT_SIZE;
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
			new_str += string::operator[](i);
		return new_str;
	}

	string::operator bool() { return is_usable; }
	char& string::operator[](size_t index) {
		if (index >= len) return null_dat;
		return data[index];
	}

	string& string::operator+=(char c) {
		if (curr_index + 2 > len) {
			void *ptr = realloc(data, curr_index + 2, &len);
			if (!ptr)
				return *this;
		}
		data[curr_index++] = c;
		data[curr_index++] = '\0';	
		return *this;
	}

	string& string::operator+=(const char *other) {
		if (!other)
			return *this;
		size_t other_len = strlen(other);
		if (curr_index + other_len + 1 >= len) {
			void *ptr = realloc(data, curr_index + other_len + 1, &len);
			if (!ptr)
				return *this;
			data = static_cast<char*>(ptr);
		}
		if (strncat(data, other, other_len))
			curr_index += other_len;
		return *this;
	}
	string& string::operator+=(const string &other) {
		if (curr_index + other.curr_index + 1 >= len) {
			void *ptr = realloc(data, curr_index + other.curr_index + 1, &len);
			if (!ptr)
				return *this;
			data = static_cast<char*>(ptr);
		}
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
}
