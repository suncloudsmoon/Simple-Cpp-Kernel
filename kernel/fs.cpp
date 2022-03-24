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

#include <lib/zstringutil.hpp>
#include <lib/zmem.hpp>
#include <lib/zmath.hpp>

namespace os {
	namespace fs {
		cactus_fs::cactus_fs(const zl::shared_ptr<os::driv::ata::atapio> &driv, LBA28 start_sector) 
							: hdd_driv(driv) {
			if (start_sector >= hdd_driv->get_dev_info().max_sector_count) {
				is_usable = false;
				return;
			}
			// TODO: add struct error {} to the class (maybe add to the standard library?)
			if (!(*is_formatted(start_sector))) {
				if (!(is_usable = *format(start_sector))) return;
			} else {
				if (!(is_usable = read_fs_info(start_sector))) return;
			}
			root_dir_loc = conv_LBA28_to_file_loc(start_sector + fs_info.info_sector_size() + 1);
		}

		bool cactus_fs::write(zl::string_view file_path, data d) {
			// zl::string a;
			return false;
		}

		/* fs_info is modified here */
		bool cactus_fs::read_fs_info(LBA28 start_sector) {
			namespace ata = os::driv::ata;
			if (!hdd_driv) return false;

			if (auto res = hdd_driv->read(ata::drive_bit::master, start_sector + 1, 1)) {
				zl::unique_ptr<uint16_t> data{ (*res).data };
				auto data_bytes = (*res).bytes;
				LBA28 fs_sec_count = 0;

				zl::memcpy<LBA28>(&fs_sec_count, 
							reinterpret_cast<LBA28*>(data.get()), sizeof(LBA28));
				zl::unique_ptr<uint8_t[]> sec_info_arr{ new uint8_t[fs_sec_count]{} };
				zl::memcpy<uint8_t>(sec_info_arr.get(), 
					reinterpret_cast<uint8_t*>(data.get() + sizeof(LBA28)),
					data_bytes - sizeof(LBA28));
				fs_info = filesystem_info{ fs_sec_count, zl::move(sec_info_arr) };
#ifdef DEBUG
	zl::cout << "Sector count: " << fs_info.max_sector << zl::endl;
#endif				
				return true;
			}
			return false;
		}

		zl::expected<bool> cactus_fs::is_formatted(LBA28 start_sector) {
			namespace ata = os::driv::ata;
			if (!hdd_driv) return {false, "[os::fs::cactus_fs::is_formatted() error] -> hdd_driv pointer is null!", -1};
			
			if (auto res = hdd_driv->read(ata::drive_bit::master, start_sector, 1)) {
				char *str = reinterpret_cast<char*>((*res).data);
				return zl::strequal(str, "cactus_fs");
			} else {
				return {false, "[os::fs::cactus_fs::is_formatted() error] -> unable to read from ATAPIO driver!", -2};
			}
		}

		/* fs_info is modified here */
		zl::expected<bool> cactus_fs::format(LBA28 start_sector) {
			namespace ata = os::driv::ata;
			if (!hdd_driv) return {false, "[os::fs::cactus_fs::format() error] -> hdd_driv pointer is null!", -1};

			char data[512] = "cactus_fs";
			ata::data_packet packet{ reinterpret_cast<uint16_t*>(data), sizeof(data) };
			if (!hdd_driv->write(ata::drive_bit::master, start_sector, packet))
				return false;

			uint32_t max_sec_count = hdd_driv->get_dev_info().max_sector_count;
			fs_info = filesystem_info{ max_sec_count, zl::unique_ptr<uint8_t[]>{ new uint8_t[max_sec_count]{} } };
			if (!fs_info.sec_info) return false;

			zl::memset<uint8_t>(fs_info.sec_info.get(), 0xFF, start_sector);	
			if (!write_filesystem_info(start_sector + 1, fs_info))
				return false;			

			file_info blank_dir{ "__blank__", file_type::dir, -1, -1 };
			LBA28 sec = (start_sector + 1) + fs_info.info_sector_size();
			file_loc file_info_loc = conv_LBA28_to_file_loc(sec);
			bool file_info_success = write_file_info(file_info_loc, blank_dir);
			
			return file_info_success;
		}

		bool cactus_fs::write_filesystem_info(LBA28 loc, filesystem_info &info) {
			namespace ata = os::driv::ata;

			LBA28 sec_arr_siz = info.max_sector + sizeof(LBA28);
			zl::unique_ptr<uint8_t> sec_arr{ new uint8_t[sec_arr_siz]{} };

			zl::memcpy<uint8_t>(sec_arr.get(), 
						reinterpret_cast<uint8_t*>(&info.max_sector),  sizeof(info.max_sector));
			zl::memcpy<uint8_t>(sec_arr.get() + sizeof(LBA28), info.sec_info.get(), info.max_sector);
			
			return hdd_driv->write(ata::drive_bit::master, loc, 
					{ reinterpret_cast<uint16_t*>(sec_arr.get()), sec_arr_siz });
		}

		bool cactus_fs::is_free_file_loc(file_loc loc) {
			auto lba_info = conv_file_loc_to_LBA28(loc);
			return (fs_info.sec_info[lba_info.sector] | zl::pow<uint8_t>(2, lba_info.section));
		}

		void cactus_fs::set_file_loc_as_busy(file_loc loc) {
			auto lba_info = conv_file_loc_to_LBA28(loc);
			fs_info.sec_info[lba_info.sector] |= zl::pow<uint8_t>(2, lba_info.section);
		}

		file_loc cactus_fs::conv_LBA28_to_file_loc(LBA28 loc) {
			return loc * 8;
		}

		file_loc_LBA28 cactus_fs::conv_file_loc_to_LBA28(file_loc loc) {
			return {loc / 8, loc % 8};
		}

		bool cactus_fs::write_file_info(file_loc loc, const file_info &info) {
			namespace ata = os::driv::ata;
			auto lba_loc = conv_file_loc_to_LBA28(loc);
			if (auto res = hdd_driv->read(ata::drive_bit::master, lba_loc.sector, 1)) {
				zl::memcpy((*res).data + (lba_loc.section * 64), &info, sizeof(file_info));
				bool write_success = hdd_driv->write(ata::drive_bit::master, lba_loc.sector, (*res));
				delete[] (*res).data;
				return write_success;
			} else {
				return false;
			}
		}

	}
}
