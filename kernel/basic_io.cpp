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

#include <kernel/basic_io.hpp>
#include <drivers/vga.hpp>

namespace os {
	static unsigned int x, y;

	void config_basic_io(unsigned int width, unsigned int height) {
		x = 0, y = 0;
		drivers::vga::config_dimensions(width, height);
		drivers::vga::clear_screen();
	}
	void putch(char c, unsigned char color_type) {
		if (c == '\n') {
			y++;
			x = 0;
			return;
		} else if (c == '\t') {
			x += drivers::vga::SPACES_PER_TAB;
			return;
		}
		if (x >= drivers::vga::get_width() * 2) {
			y++;
			x = 0;
		}
		if (y >= drivers::vga::get_height())
			return;

		auto pos = drivers::vga::map_to_1D(x, y, drivers::vga::get_width() * 2);
		os_vga_ptr[pos] = c;
		os_vga_ptr[pos + 1] = color_type;
		x += 2;
	}
}
