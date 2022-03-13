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

#ifndef DRIVERS_ATA_HPP
#define DRIVERS_ATA_HPP

#include <stddef.h>
#include <stdint.h>

#include <lib/zutil.hpp>
#include <lib/zstring.hpp>

namespace os {
	namespace driv {
		namespace ata {
			using LBA28 = uint32_t;
			namespace commands {
				constexpr uint16_t identity = 0xEC;
				constexpr uint8_t read_sec = 0x20;
				constexpr uint8_t write_sec = 0x30;
			}
			namespace ports {
				constexpr uint16_t io_base = (uint16_t) 0x1F0;
				constexpr uint16_t data = io_base + (uint16_t) 0;
				constexpr uint16_t err = io_base + (uint16_t) 1;
				
				constexpr uint16_t sec_count = io_base + (uint16_t) 2;
				constexpr uint16_t lba_low = io_base + (uint16_t) 3;
				constexpr uint16_t lba_mid = io_base + (uint16_t) 4;
				constexpr uint16_t lba_high = io_base + (uint16_t) 5;

				constexpr uint16_t drive_head_select = io_base + (uint16_t) 6;
				constexpr uint16_t command = io_base + (uint16_t) 7;
				constexpr uint16_t status = command;
			}
			// Useful once you get the status value from ports::status
			namespace status_info {
				constexpr uint8_t err = (uint8_t) 1;
				constexpr uint8_t idx = (uint8_t) 2;
				constexpr uint8_t corr = (uint8_t) 4;
				constexpr uint8_t drq = (uint8_t) 8;
				constexpr uint8_t srv = (uint8_t) 16;
				constexpr uint8_t df = (uint8_t) 32;
				constexpr uint8_t rdy = (uint8_t) 64;
				constexpr uint8_t bsy = (uint8_t) 128;
			}

			namespace drive_type {
				enum {
					master_drive = 0xE0,
					slave_drive = 0xF0
				};
			}
			// Got info from https://wiki.osdev.org/PCI_IDE_Controller#Read.2FWrite_From_ATA_Drive
			namespace drive_bit {
				enum {
					master_bit = 0,
					slave_bit = 1
				};
			}

			struct data_packet {
				data_packet(uint16_t *dat, size_t siz) : data(dat), size(siz) {}
				operator bool() { return data && size; }
				uint16_t *data;
				size_t size;
			};
			struct atapio_identify_info {
				LBA28 max_sector_count = 0;
			};
			struct err_message {
				zl::string str;
				int code = 0;
			};
			class atapio {
				public:
					atapio(unsigned polling_limit = 1000);
					operator bool() { return ata_init_success; }
					
					zl::expected<data_packet> read(int drive_bit, LBA28 addr, uint16_t num_sectors);
					bool write(int drive_bit, LBA28 addr, data_packet dat);
					
					const atapio_identify_info& get_dev_info() const {
						return hdd_info;
					}
					const err_message& get_error_message() const {
						return err;
					}
				private:
					void identity_cmd();
					unsigned poll_lim;
					bool ata_init_success = true;
					
					atapio_identify_info hdd_info;
					err_message err;
			};
		}
	}
}

#endif /* DRIVERS_ATA_HPP */