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

#ifndef LIB_ZMATH_HPP
#define LIB_ZMATH_HPP

#include <drivers/instr.hpp>

namespace zl {
	template<typename T>
	inline T round(T val, T nearest) {
		return (val / nearest) * nearest + nearest;
	}
	inline double sin(double x) {
		return os::drivers::instr::fsin(x);
	}
	inline double cos(double x) {
		return os::drivers::instr::fcos(x);
	}
	inline double tan(double x) {
		return os::drivers::instr::fptan(x);
	}
	inline double remainder(double dividend, double divisor) {
		return os::drivers::instr::fprem1(dividend, divisor);
	}
	// TODO: change this
	inline double mod(double dividend, double divisor) {
		return os::drivers::instr::fprem1(dividend, divisor);
	}
}

#endif /* LIB_ZMATH_HPP */