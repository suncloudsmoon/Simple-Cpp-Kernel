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
#include <kernel/fs.hpp>

// Unit testing
#include <test/test_zstring.hpp>
#include <lib/zmath.hpp>
#include <lib/shared_ptr.hpp>

namespace {
	void config_os() {
		os::config_basic_io(80, 25);
		zl::config_zio();
		os::mem::init();
	}
}

extern "C" void main() {
	using namespace os::driv::ata;
	config_os();

	zl::cout << "Pointer #1: " << zl::unique_ptr<int[]>{ new int[5]{} } << zl::endl;
	zl::cout << "Pointer #2: " << zl::unique_ptr<int[]>{ new int[5]{} } << zl::endl;
	zl::cout << "Pointer #3: " << zl::unique_ptr<int[]>{ new int[5]{} } << zl::endl;
	
	zl::cout << "String equals: " << (zl::string("hello") == zl::string("hello")) << zl::endl;
	// String split testing
	zl::string a = "hellowowhiwow";
	// auto loc = a.find("wow");
	// zl::cout << "Loc is " << loc << zl::endl;
	auto res = a.split("wow");
	for (auto ele : res) {
		zl::cout << ele << ",";
	}
	zl::cout << zl::endl;
	
	zl::shared_ptr<atapio> atapio_ptr{ new atapio{} };
	if (atapio_ptr->operator bool()) {
		os::fs::cactus_fs files{ atapio_ptr };
		zl::cout << "----------" << "Filesystem log" << "----------\n";
		zl::cout << "Ata init success: " << files << zl::endl;
		zl::cout << "----------" << "End of filesystem log" << "----------\n";
	} else {
		zl::cerr << "Failed to initialize ata driver!" << zl::endl;
	}
}
