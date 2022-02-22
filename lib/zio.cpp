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
#include <lib/zio.hpp>
#include <kernel/basic_io.hpp>

namespace zl {
	static size_t index;
	ostream& ostream::operator<<(color_type type) {
		color = type;
		return *this;
	}

	ostream& ostream::operator<<(bool b) {
		(b) ? operator<<("true") : operator<<("false");
		return *this;
	}
	ostream& ostream::operator<<(unsigned short a) {
		operator<<(*zl::itoa(a));
		return *this;
	}
	ostream& ostream::operator<<(short a) {
		operator<<(*zl::itoa(a));
		return *this;
	}
	ostream& ostream::operator<<(unsigned int a) {
		operator<<(*zl::itoa(a));
		return *this;
	}
	ostream& ostream::operator<<(int a) {
		operator<<(*zl::itoa(a));
		return *this;
	}
	ostream& ostream::operator<<(unsigned long a) {
		operator<<(*zl::itoa(a));
		return *this;
	}
	ostream& ostream::operator<<(long a) {
		operator<<(*zl::itoa(a));
		return *this;
	}

	ostream& ostream::operator<<(char c) {
		os::putch(c, color.get_color_num());
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
	ostream cerr;

	void config_zio() {
		index = 0;
		cout = ostream();
		cerr = ostream(color::red);
	}
}
