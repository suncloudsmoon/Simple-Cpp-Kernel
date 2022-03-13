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

#include <drivers/ata.hpp>
#include <drivers/instr.hpp>

#include <lib/zmem.hpp>
#include <lib/zio.hpp>
#include <lib/zassert.hpp>

namespace os {
	namespace driv {
		namespace ata {
			atapio::atapio(unsigned polling_limit) : poll_lim(polling_limit) {
				identity_cmd();
			}

			void atapio::identity_cmd() {
				x86::outb(ports::drive_head_select, drive_type::master_drive);
				// Zero out the IO ports
				for (uint16_t port = 0x1F2; port <= 0x1F5; port++)
					x86::outb(port, NULL);
				x86::outb(ports::command, commands::identity);
				
				// Polling for the status
				unsigned polling_index = 0;
				uint16_t status = 0;
				while (polling_index++ < poll_lim) {
					status = x86::inb(ports::status);
					if ((status & 128) == 0) break;
				}
				if (status == 0) {
					ata_init_success = false;
					err.str = "[os::driv::ata::atapio::identity_cmd() error] -> status poll failure!";
					err.code = -1;
					return;
				}

				// Read 256 16 bit values from the data (0x1F0) port
				uint16_t identity_data[256]{};
				for (uint16_t i = 0; i < 256; i++)
					identity_data[i] = x86::inw(ports::data);
				
				uint32_t first_lba_max = identity_data[60];
				uint32_t second_lba_max = identity_data[61] << 16;
				hdd_info.max_sector_count |= first_lba_max;
				hdd_info.max_sector_count |= second_lba_max;
				if (hdd_info.max_sector_count == 0) {
					ata_init_success = false;
					err.str = "[os::driv::ata::atapio::identity_cmd() error] -> LBA28 addressing mode is not supported on current hardware!";
					err.code = -2;
				}
			}

			// TODO: fix memory leak resulting from data_packet... maybe add a destructor?
			// num_sectors -> number of 512 byte sectors to read
			zl::expected<data_packet> atapio::read(int drive_bit, LBA28 addr, uint16_t num_sectors) {
				zl::assert(ata_init_success, "[os::driv::ata::atapio::read(int, LBA28, uint16_t) error] -> ATA failed to initialize!");
				
				uint16_t num_read = num_sectors * 256;
				data_packet packet = { new uint16_t[num_read](), num_read };

				x86::outb(ports::sec_count, num_sectors / 256);
				x86::outb(ports::lba_low, addr);
				x86::outb(ports::lba_mid, addr >> 8);
				x86::outb(ports::lba_high, addr >> 16);
				x86::outb(ports::drive_head_select, drive_type::master_drive | (drive_bit << 4) | ((addr >> 24) & 0xF));
				x86::outb(ports::command, commands::read_sec);

				// Wait until the device is ready to tranfer data
				unsigned poll_index = 0;
				while (poll_index++ < poll_lim) {
					uint8_t status = x86::inb(ports::status);
					// (Bit 3 is not 0 and Bit 7 is 0) or (Bit 0/5 is 1)
					if ((status & status_info::drq) != 0 && (status & status_info::bsy) == 0) break;
				}
				if (poll_index >= poll_lim)
					return {{nullptr, 0}, "[os::driv::ata::atapio::read() error] -> unable to read from ATA bus within polling limit!", -1};
				for (size_t i = 0; i < packet.size; i++)
					packet.data[i] = x86::inw(ports::data);

				return packet;
			}
			bool atapio::write(int drive_bit, LBA28 addr, data_packet dat) {
				zl::assert(ata_init_success, "[os::driv::ata::atapio::read(int, LBA28, uint16_t) error] -> ATA failed to initialize!");
				
				if (!dat) return false;

				x86::outb(ports::sec_count, (dat.size / 256) / 256);
				x86::outb(ports::lba_low, addr);
				x86::outb(ports::lba_mid, addr >> 8);
				x86::outb(ports::lba_high, addr >> 16);
				x86::outb(ports::drive_head_select, drive_type::master_drive | (drive_bit << 4) | ((addr >> 24) & 0xF));
				x86::outb(ports::command, commands::write_sec);

				// Wait until the device is ready to tranfer data
				unsigned poll_index = 0;
				while (poll_index++ < poll_lim) {
					uint8_t status = x86::inb(ports::status);
					// (Bit 3 is not 0 and Bit 7 is 0) or (Bit 0/5 is 1)
					if ((status & status_info::drq) != 0 && (status & status_info::bsy) == 0) break;
				}
				if (poll_index >= poll_lim)
					return false;
				for (size_t i = 0; i < dat.size; i++)
					x86::outw(ports::data, dat.data[i]);

				return true;		
			}
		}
	}
}