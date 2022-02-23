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

extern "C" int os_drivers_instr_insw(uint16_t port, char *buf);
extern "C" int os_drivers_instr_inb(uint16_t port);
extern "C" void os_drivers_instr_outb(uint16_t port, uint16_t dat);

namespace os {
	namespace drivers {
		namespace instr {
			inline int inb(uint16_t port) {
				return os_drivers_instr_inb(port);
			}
			inline void outb(uint16_t port, uint16_t dat) {
				os_drivers_instr_outb(port, dat);
			}
		}
	}
}

#endif /* DRIVERS_INSTR_HPP */