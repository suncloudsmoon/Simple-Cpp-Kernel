#ifndef DRIVERS_ATA_HPP
#define DRIVERS_ATA_HPP

#include <stdint.h>
#include <stddef.h>

namespace os {
	namespace drivers {
		namespace commands {
			constexpr uint16_t identity = 0xEC;
			constexpr uint8_t read_sec = 0x20;
			constexpr uint8_t write_sec = 0x30;
		}
		namespace ports {
			constexpr uint8_t io_base = (uint8_t) 0x1F0;
			constexpr uint16_t data = io_base + (uint16_t) 0;
			constexpr uint16_t err = io_base + (uint16_t) 1;
			
			constexpr uint16_t sec_count = io_base + (uint16_t) 2;
			constexpr uint16_t sec_num = io_base + (uint16_t) 3;
			constexpr uint16_t cylin_low = io_base + (uint16_t) 4;
			constexpr uint16_t cylin_high = io_base + (uint16_t) 4;

			constexpr uint8_t drive_head_select = io_base + (uint8_t) 6; // 8 bit output
			constexpr uint8_t command = io_base + (uint8_t) 7; // 8 bit output
			constexpr uint8_t status = io_base + (uint8_t) 7; // 8 bit output
		}
		namespace drive_type {
			enum {
				master_drive = 0xA0,
				slave_drive = 0xB0
			};
		}

		// Got info from https://wiki.osdev.org/PCI_IDE_Controller#Read.2FWrite_From_ATA_Drive
		namespace drive_bit {
			enum {
				master_bit = 0,
				slave_bit = 1
			};
		}

		// for ata::write() function
		struct CHS {
			uint16_t cylinder;
			uint16_t head;
			uint16_t sector;
		};
		struct data_packet {
			data_packet(uint16_t *dat, size_t siz) : data(dat), size(siz) {}
			uint16_t *data;
			size_t size;
			operator bool() { return data && size; }
		};

		class ata {
			public:
				ata(unsigned poll_lim = 1000);
				~ata();
				operator bool() { return ata_init_success; }
				data_packet read(int drive_bit, CHS addr, uint16_t num_sectors);
				bool write(int drive_bit, CHS addr, data_packet dat);
			private:
				void identity_cmd();
				bool ata_init_success;
				unsigned polling_limit;	
		};
	}
}

#endif /* DRIVERS_ATA_HPP */