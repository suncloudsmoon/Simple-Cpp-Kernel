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

#include <kernel/fs.hpp>

namespace os {
	namespace fs {
		cactus_fs::cactus_fs(const zl::shared_ptr<os::driv::ata::atapio> &driv, LBA28 start_loc) 
							: hdd_driv(driv) {
			if (start_loc > hdd_driv->get_dev_info().max_sector_count) {
				is_usable = false;
				return;
			}
			start_sector = start_loc;
		}

		bool cactus_fs::is_formatted() {
			if (!hdd_driv) return false;

			if (auto res = hdd_driv->read(os::driv::ata::drive_bit::master_bit, start_sector, 1)) {
				char *str = reinterpret_cast<char*>((*res).data);
				delete[] (*res).data;
			} else {
				return false;
			}
			return true; 
		}
		void cactus_fs::format() {

		}

	}
}
