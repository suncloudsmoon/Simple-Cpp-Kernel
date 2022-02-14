#ifndef LIB_ZIO_HPP
#define LIB_ZIO_HPP

#include <kernel/basic_io.hpp>
#include <lib/zstring.hpp>

namespace zl {
	struct color_type {
		constexpr color_type(int color_type) : color(color_type) {}
		constexpr int get_color_num() const { return color; }
	private:
		int color;
	};
	namespace color {
		constexpr color_type red{OS_VGA_COLOR_RED};
	}
	struct ostream {
		ostream(color_type type = OS_VGA_COLOR_WHITE) : color(type) {}
		ostream& operator<<(color_type type);
		ostream& operator<<(bool b);
		ostream& operator<<(char c);
		ostream& operator<<(const char *str);
		ostream& operator<<(const zl::string &str);
	private:
		color_type color;
	};

	constexpr char endl{'\n'};
	extern ostream cout;
	extern ostream cerr;
}

#endif /* LIB_ZIO_HPP */