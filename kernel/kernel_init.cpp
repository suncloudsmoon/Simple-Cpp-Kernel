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

#include <kernel/kernel_init.hpp>
#include <lib/zio.hpp>
#include <lib/zmem.hpp>
#include <lib/zstring.hpp>
#include <lib/zstringutil.hpp>

// OS stuff
#include <kernel/basic_io.hpp>
#include <drivers/ata.hpp>

// Unit testing
#include <test/test_zstring.hpp>
#include <lib/zmath.hpp>

static void config_os();

extern "C" void main() {
	config_os();

	os::drivers::ata driv;
	if (driv) {
		auto res = driv.read(os::drivers::drive_bit::master_bit, {0, 0, 2}, 1);
		for (size_t i = 0; i < 100; i++)
			zl::cout << (unsigned short) res.data[i] << ",";
		zl::cout << zl::endl;
	} else {
		zl::cerr << "Unable to initialize ATA driver!" << zl::endl;
	}

	// double num = 25.0;
	// int c = zl::mod(num, 10);
	// zl::cout << (zl::mod(28.0, 5) <= -2.0) << zl::endl;
}

static void config_os() {
	os::config_basic_io(80, 25);
	zl::config_zio();
    os::mem_init();
}