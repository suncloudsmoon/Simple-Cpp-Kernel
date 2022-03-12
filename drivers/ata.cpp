#include <drivers/ata.hpp>
#include <drivers/instr.hpp>

#include <lib/zmem.hpp>
#include <lib/zio.hpp>
#include <lib/zassert.hpp>

namespace os {
	namespace drivers {
		ata::ata(unsigned poll_lim) : ata_init_success(false), polling_limit(poll_lim) {
			identity_cmd();
		}
		ata::~ata() {

		}

		// TODO: fix memory leak resulting from data_packet... maybe add a destructor?
		data_packet ata::read(int drive_bit, CHS addr, uint16_t num_sectors) {
			uint16_t num_read = num_sectors * 256;
			data_packet packet = { new uint16_t[num_read](), num_read };

			instr::outb(ports::sec_count, num_sectors);
			instr::outb(ports::sec_num, addr.sector);
			instr::outb(ports::cylin_low, addr.cylinder & 0xFF); // zero out 0000 0000 1111 1111
			instr::outb(ports::cylin_high, addr.cylinder >> 8);
			instr::outb(ports::drive_head_select, drive_type::master_drive | (drive_bit << 4) | addr.head);

			instr::outb(ports::command, commands::read_sec);

			for (size_t i = 0; i < packet.size; i++)
				packet.data[i] = instr::inw(ports::data);

			return packet;
		}
		bool ata::write(int drive_bit, CHS addr, data_packet dat) {
			if (!dat) return false;
			
			instr::outb(ports::sec_count, (dat.size * sizeof(uint16_t)) / 512);
			instr::outb(ports::sec_num, addr.sector);
			instr::outb(ports::cylin_low, addr.cylinder & 0xFF); // zero out 0000 0000 1111 1111
			instr::outb(ports::cylin_high, addr.cylinder & 0xFF00);
			instr::outb(ports::drive_head_select, drive_type::master_drive | (drive_bit << 4) | addr.head);

			// changed from outb to outw
			instr::outb(ports::command, commands::write_sec);

			for (size_t i = 0; i < dat.size; i++)
				instr::outw(ports::data, dat.data[i]);

			return true;
		}

		void ata::identity_cmd() {
			instr::outb(ports::drive_head_select, drive_type::master_drive);
			// Zero out the IO ports
			for (uint16_t port = 0x1F2; port <= 0x1F5; port++)
				instr::outb(port, NULL);
			instr::outb(ports::command, commands::identity);
			
			// Polling for the status
			unsigned polling_index = 0;
			uint16_t status;
			while (polling_index++ < polling_limit) {
				status = instr::inb(ports::status);
				if ((status & 128) == 0) break;
			}
			if (status == 0) {
				ata_init_success = false;
				return;
			}

			// Read 256 16 bit values from the data (0x1F0) port
			uint16_t identity_data[256]{};
			for (uint16_t i = 0; i < 256; i++)
				identity_data[i] = instr::inw(ports::data);

#ifdef DEBUG
	zl::cout << "Ata Driver Testing" << zl::endl;
	zl::cout << "Polling status is " << status << zl::endl;
	for (int i = 0; i < 100; i++)
		zl::cout << "[" << i << "]: " << identity_data[i] << ",";
	zl::cout << zl::endl;	
	zl::cout << "End of Ata Driver Testing" << zl::endl;
#endif
		}
	}
}