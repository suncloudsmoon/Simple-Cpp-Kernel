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

#ifndef DRIVERS_INSTR_HPP
#define DRIVERS_INSTR_HPP

#include <stdint.h>

namespace os::driv {
	namespace x86 {
		// General x86 instructions
		void hlt();

		// I/O operations
		uint8_t inb(uint16_t port);
		void outb(uint16_t port, uint8_t dat);
		
		uint16_t inw(uint16_t port);
		void outw(uint16_t port, uint16_t dat);
	}
	// FPU stuff
	namespace x87 {
		double fsin(double x);
		double fcos(double x);
		double fptan(double x);
		double fprem1(double dividend, double divisor);
	}
}

#endif /* DRIVERS_INSTR_HPP */