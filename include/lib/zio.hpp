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

#ifndef LIB_ZIO_HPP
#define LIB_ZIO_HPP

#include <stdint.h>

#include <drivers/vga.hpp>

namespace zl {
	struct color_type {
		constexpr color_type(int color_type) : color(color_type) {}
		constexpr int get_color_num() const { return color; }
	private:
		int color;
	};
	namespace color {
		constexpr color_type black = os::drivers::vga::black;
		constexpr color_type blue = os::drivers::vga::blue;
		constexpr color_type green = os::drivers::vga::green;
		constexpr color_type cyan = os::drivers::vga::cyan;
		constexpr color_type red = os::drivers::vga::red;
		constexpr color_type purple = os::drivers::vga::purple;
		constexpr color_type brown = os::drivers::vga::brown;
		constexpr color_type gray = os::drivers::vga::gray;
		constexpr color_type dark_gray = os::drivers::vga::dark_gray;
		constexpr color_type yellow = os::drivers::vga::yellow;
		constexpr color_type white = os::drivers::vga::white;
		
		constexpr color_type light_blue = os::drivers::vga::light_blue;
		constexpr color_type light_green = os::drivers::vga::light_green;
		constexpr color_type light_cyan = os::drivers::vga::light_cyan;
		constexpr color_type light_red = os::drivers::vga::light_red;
		constexpr color_type light_purple = os::drivers::vga::light_purple;
	}
	struct ostream {
		ostream(color_type type = color::white) : color(type) {}
		ostream& operator<<(color_type type);
		
		ostream& operator<<(bool b);
		ostream& operator<<(unsigned short a);
		ostream& operator<<(short a);
		ostream& operator<<(unsigned int a);
		ostream& operator<<(int a);
		ostream& operator<<(unsigned long a);
		ostream& operator<<(long a);
		
		ostream& operator<<(char c);
		ostream& operator<<(const char *str);
	private:
		color_type color;
	};

	constexpr char endl = '\n';
	extern ostream cout;
	extern ostream cerr;

	void config_zio();
}

#endif /* LIB_ZIO_HPP */