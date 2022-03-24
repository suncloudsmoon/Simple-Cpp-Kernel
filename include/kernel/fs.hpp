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
#include <stddef.h>

#include <lib/shared_ptr.hpp>
#include <lib/unique_ptr.hpp>
#include <lib/expected.hpp>
#include <lib/string_view.hpp>

#include <drivers/ata.hpp>

namespace os {
	namespace fs {
		using LBA28 = os::driv::ata::LBA28;
		using file_loc = LBA28;
		namespace file_type {
			enum { dir = 0, file = 1 };
		}
		struct filesystem_info {
			LBA28 max_sector;
			zl::unique_ptr<uint8_t[]> sec_info;
			size_t info_size() const { return max_sector + 1; }
			size_t info_sector_size() const { return (info_size()) / 513 + 1; }
		};
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
		struct file_loc_LBA28 {
			uint32_t sector;
			uint32_t section; // 1 to 8
		};
		struct data {
			void *ptr;
			size_t bytes;
		};
		class cactus_fs {
		public:	
			cactus_fs(const zl::shared_ptr<os::driv::ata::atapio> &driv, LBA28 start_sector = 195);
			operator bool() const { return is_usable; }
			
			bool write(zl::string_view file_path, data d);
			bool read_fs_info(LBA28 start_sector);
			zl::expected<bool> is_formatted(LBA28 start_sector);
			zl::expected<bool> format(LBA28 start_sector);
			zl::expected<bool> get_sec_info();
		private:
			bool write_filesystem_info(LBA28 loc, filesystem_info &info);

			bool is_free_file_loc(file_loc loc);
			void set_file_loc_as_busy(file_loc loc);

			file_loc conv_LBA28_to_file_loc(LBA28 loc);
			file_loc_LBA28 conv_file_loc_to_LBA28(file_loc loc);
			
			bool write_file_info(file_loc loc, const file_info &info);

			zl::shared_ptr<os::driv::ata::atapio> hdd_driv;
			filesystem_info fs_info;
			file_loc root_dir_loc = -1;
			bool is_usable = true;
		};
	}
}

#endif /* KERNEL_FS_HPP */