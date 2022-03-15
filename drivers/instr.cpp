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

#include <drivers/instr.hpp>

namespace os::driv {
	namespace x86 {
		void hlt() {
			asm("hlt");
		}

		uint8_t inb(uint16_t port) {
#ifdef __GNUC__					
			uint8_t dat;
			asm volatile(
				"inb %1, %0"
				: "=a"(dat)
				: "d"(port)
			);
			return dat;
#else
#error "Your compiler is not supported for executing inw(port) function yet!"
#endif				
		}

		void outb(uint16_t port, uint8_t dat) {
#ifdef __GNUC__
			asm volatile(
				"outb %1, %0"
				:
				: "d"(port), "a"(dat)
			);
#else
#error "Your compiler is not supported for executing inw(port) function yet!"
#endif
		}

		uint16_t inw(uint16_t port) {
#ifdef __GNUC__				
			uint16_t dat;
			asm volatile (
				"inw %1, %0"
				: "=a"(dat)
				: "d"(port)
			);
			return dat;
#else
#error "Your compiler is not supported for executing inw(port) function yet!"
#endif
		}

		void outw(uint16_t port, uint16_t dat) {
#ifdef __GNUC__				
			asm volatile(
				"outw %0, %1"
				:
				: "a"(dat), "d"(port)
			);
#else
#error "Your compiler is not supported for executing outw(port) function yet!"
#endif				
		}
	}
	namespace x87 {
		// Assembly trig functions
		double fsin(double x) {
#ifdef __GNUC__				
			double res;
			asm(
				"fldl %1;"
				"fsin;"
				"fstl %0;"
				: "=m"(res)
				: "m"(x)
			);
			return res;
#else
#error "Your compiler is not supported for executing fsin(x) function yet!"
#endif				
		}

		double fcos(double x) {
#ifdef __GNUC__				
			double res;
			asm(
				"fldl %1;"
				"fcos;"
				"fstl %0;"
				: "=m"(res)
				: "m"(x)
			);
			return res;
#else
#error "Your compiler is not supported for executing fcos(x) function yet!"
#endif
		}

		double fptan(double x) {
#ifdef __GNUC__				
			double res;
			asm(
				"fldl %1;"
				"fptan;"
				"fstl %0;"
				: "=m"(res)
				: "m"(x)
			);
			return res;
#else
#error "Your compiler is not supported for executing fptan(x) function yet!"
#endif
		}
		double fprem1(double dividend, double divisor) {
#ifdef __GNUC__				
			double res;
			asm(
				"fldl %1;"
				"fldl %2;"
				"fprem1;"
				"fstl %0;"
				: "=m"(res)
				: "m"(divisor), "m"(dividend)
			);
			return res;
#else
#error "Your compiler is not supported for executing fptan(x) function yet!"
#endif	
		}
	}
}