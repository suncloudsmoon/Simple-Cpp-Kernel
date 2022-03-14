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
#include <lib/shared_ptr.hpp>

namespace {
	void config_os() {
		os::config_basic_io(80, 25);
		zl::config_zio();
		os::mem::init();
	}
}

extern "C" void main() {
	config_os();

	os::driv::ata::atapio hdd;
	if (hdd) {
		using LBA28 = os::driv::ata::LBA28;
		constexpr LBA28 sector_num = 20;
		
		uint16_t str[256]{};
		for (size_t i = 0; i < 256; i++)
			str[i] = 100;
		os::driv::ata::data_packet packet{str, sizeof(str)};
		bool write_success = hdd.write(os::driv::ata::drive_bit::master_bit, sector_num, packet);
		zl::cout << "Write success: " << write_success << zl::endl;
		
		if (auto res = hdd.read(os::driv::ata::drive_bit::master_bit, sector_num, 1)) {
			zl::cout << "Data from HDD: " << zl::endl;
			for (size_t i = 0; i < (*res).bytes / sizeof(uint16_t); i++)
				zl::cout << (*res).data[i] << ",";
			zl::cout << zl::endl;	
			delete[] (*res).data;
		} else {
			zl::cerr << "ATA Error: " << res.get_err_message() << zl::endl;
		}
	} else {
		zl::cerr << "ATA Error: " << hdd.get_err().str << zl::endl;
	}
}
