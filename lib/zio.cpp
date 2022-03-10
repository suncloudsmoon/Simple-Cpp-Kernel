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

#include <stddef.h>

#include <kernel/basic_io.hpp>
#include <lib/zio.hpp>

namespace zl {
	namespace {
		template<typename T, typename Func>
		void print_out_whole_num(T a, Func operation) {
			if (a == 0) {
				operation('0');
				return;
			}
			constexpr size_t num_bits = 8 * sizeof(T);
			char arr[num_bits];
			int index = num_bits - 1;

			while ((index >= 0) && (a != 0)) {
				char c = (a % 10) + '0';
				arr[index] = c;
				a /= 10;
				index--;
			}
			for (int i = index + 1; i < num_bits; i++)
				operation(arr[i]);
		}
	}
	ostream& ostream::operator<<(color_type type) {
		color = type;
		return *this;
	}

	ostream& ostream::operator<<(bool b) {
		return (b) ? operator<<("true") : operator<<("false");
	}
	ostream& ostream::operator<<(unsigned short a) {
		print_out_whole_num(a, [&](char c) { operator<<(c); });
		return *this;
	}
	ostream& ostream::operator<<(short a) {
		print_out_whole_num(a, [&](char c) { operator<<(c); });
		return *this;
	}
	ostream& ostream::operator<<(unsigned int a) {
		print_out_whole_num(a, [&](char c) { operator<<(c); });
		return *this;
	}
	ostream& ostream::operator<<(int a) {
		print_out_whole_num(a, [&](char c) { operator<<(c); });
		return *this;
	}
	ostream& ostream::operator<<(unsigned long a) {
		print_out_whole_num(a, [&](char c) { operator<<(c); });
		return *this;
	}
	ostream& ostream::operator<<(long a) {
		print_out_whole_num(a, [&](char c) { operator<<(c); });
		return *this;
	}

	ostream& ostream::operator<<(char c) {
		os::putch(c, color.get_color_num());
		return *this;
	}
	ostream& ostream::operator<<(const char* str) {
		if (!str) return operator<<("nullptr");
		size_t index = 0;
		while (char c = str[index++]) { operator<<(c); }
		return *this;
	}

	ostream cout;
	ostream cerr;

	void config_zio() {
		cout = ostream();
		cerr = ostream(color::red);
	}
}
