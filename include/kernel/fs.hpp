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

#ifndef KERNEL_FS_HPP
#define KERNEL_FS_HPP

#include <stdint.h>

#include <lib/shared_ptr.hpp>

#include <drivers/ata.hpp>

namespace os {
	namespace fs {
		using LBA28 = os::driv::ata::LBA28;
		constexpr const char *fs_name = "cactus_fs";
		struct file_info {
			char file_name[59];
			uint8_t type;
			int32_t file_loc;
			int32_t next_dat_loc;
		};
		struct file_data {
			int32_t next_dat_loc;
			uint16_t dat[254];
		};
		class cactus_fs {
			cactus_fs(const zl::shared_ptr<os::driv::ata::atapio> &driv, LBA28 start_loc = 195);
			operator bool() const { return is_usable; }
			bool is_formatted();
			void format();
		private:
			zl::shared_ptr<os::driv::ata::atapio> hdd_driv;
			LBA28 start_sector;
			bool is_usable = true;
		};
	}
}

#endif /* KERNEL_FS_HPP */