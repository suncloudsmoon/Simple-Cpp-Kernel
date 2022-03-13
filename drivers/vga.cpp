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

#include <drivers/vga.hpp>

namespace os {
	namespace driv {
		namespace vga {
			static unsigned int width, height;
			void config_dimensions(unsigned int w, unsigned int h) {
				width = w;
				height = w;
			}
			unsigned int get_width() {
				return width;
			}
			unsigned int get_height() {
				return height;
			}
			void clear_screen() {
				for (unsigned int y = 0; y < height; y++) {
					for (unsigned int x = 0; x < width * 2; x += 2) {
						auto pos = map_to_1D(x, y, width);
						os_vga_ptr[pos] = '\0';
						os_vga_ptr[pos + 1] = white;
					}
				}
			}
		}
	}
}