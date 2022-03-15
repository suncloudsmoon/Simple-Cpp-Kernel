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

#ifndef DRIVERS_VGA_HPP
#define DRIVERS_VGA_HPP

namespace os::driv::vga {
	// Since volatile pointers cannot be declared as constexpr, I chose this macro instead
	#define os_vga_ptr reinterpret_cast<volatile char *>(0xB8000)
	constexpr unsigned spaces_per_tab = 4;
	enum {
		black = 0,
		blue = 1,
		green = 2,
		cyan = 3,
		red = 4,
		purple = 5,
		brown = 6,
		gray = 7,
		dark_gray = 8,
		light_blue = 9,
		light_green = 10,
		light_cyan = 11,
		light_red = 12,
		light_purple = 13,
		yellow = 14,
		white = 15
	};
	constexpr unsigned int map_to_1D(unsigned int x, unsigned int y, unsigned int num_rows) {
		return (y * num_rows) + x;
	}
	void config_dimensions(unsigned int w, unsigned int h);
	unsigned int get_width();
	unsigned int get_height();
	void clear_screen();
}

#endif /* DRIVERS_VGA_HPP */