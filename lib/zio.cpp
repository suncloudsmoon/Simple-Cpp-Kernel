#include <stddef.h>
#include <lib/zio.hpp>

namespace zl {
	static size_t index{0};
	ostream& ostream::operator<<(color_type type) {
		color = type;
		return *this;
	}
	ostream& ostream::operator<<(bool b) {
		if (b)
			operator<<("true");
		else
			operator<<("false");
		return *this;
	}
	ostream& ostream::operator<<(char c) {
		os_putch(c, color.get_color_num());
		return *this;
	}
	ostream& ostream::operator<<(const char* str) {
		if (!str) {
			operator<<("nullptr");
			return *this;
		}
		while (char c = str[index++]) { operator<<(c); }
			return *this;
	}

	ostream& ostream::operator<<(const zl::string &str) {
		return operator<<(str.c_str());
	}
	ostream cout;
	ostream cerr{OS_VGA_COLOR_RED};
}
