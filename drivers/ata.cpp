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
			atapio::atapio(unsigned polling_limit) : ata_init_success(true), poll_lim(polling_limit) {
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
					return;
				}

				// Read 256 16 bit values from the data (0x1F0) port
				uint16_t identity_data[256]{};
				for (uint16_t i = 0; i < 256; i++)
					identity_data[i] = x86::inw(ports::data);
	#ifdef DEBUG
		zl::cout << "Ata Driver Testing" << zl::endl;
		zl::cout << "Polling status is " << status << zl::endl;
		for (int i = 0; i < 100; i++)
			zl::cout << "[" << i << "]: " << identity_data[i] << ",";
		zl::cout << zl::endl;	
		zl::cout << "End of Ata Driver Testing" << zl::endl;
	#endif
			}

			// TODO: fix memory leak resulting from data_packet... maybe add a destructor?
			zl::expected<data_packet> atapio::read(int drive_bit, CHS addr, uint16_t num_sectors) {
				uint16_t num_read = num_sectors * 256;
				data_packet packet = { new uint16_t[num_read](), num_read };

				x86::outb(ports::sec_count, num_sectors);
				x86::outb(ports::sec_num, addr.sector);
				x86::outb(ports::cylin_low, addr.cylinder & 0xFF); // zero out 0000 0000 1111 1111
				x86::outb(ports::cylin_high, addr.cylinder >> 8);
				x86::outb(ports::drive_head_select, drive_type::master_drive | (drive_bit << 4) | addr.head);
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
			bool atapio::write(int drive_bit, CHS addr, data_packet dat) {
				if (!dat) return false;
				
				x86::outb(ports::sec_count, (dat.size * sizeof(uint16_t)) / 512);
				x86::outb(ports::sec_num, addr.sector);
				x86::outb(ports::cylin_low, addr.cylinder & 0xFF); // zero out 0000 0000 1111 1111
				x86::outb(ports::cylin_high, addr.cylinder & 0xFF00);
				x86::outb(ports::drive_head_select, drive_type::master_drive | (drive_bit << 4) | addr.head);
				x86::outb(ports::command, commands::write_sec);

				// Wait until the device is ready to tranfer data
				unsigned poll_index = 0;
				while (poll_index++ < poll_lim) {
					uint16_t status = x86::inw(ports::status);
					// (Bit 3 and 7) must be 0 or (Bit 0 or 5) is 1
					if (((status & 8) == 0 && (status & 128) == 0) || ((status & 1) != 0 || (status & 32) != 0)) break;
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