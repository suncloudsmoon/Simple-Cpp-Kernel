#ifndef DRIVERS_INSTR_HPP
#define DRIVERS_INSTR_HPP

#include <stdint.h>

extern "C" int os_drivers_instr_insw(uint16_t port, char *buf);
extern "C" int os_drivers_instr_inb(uint16_t port);
extern "C" void os_drivers_instr_outb(uint16_t port, uint16_t dat);

namespace os {
	namespace drivers {
		namespace instr {
			inline int inb(uint16_t port) {
				return os_drivers_instr_inb(port);
			}
			inline void outb(uint16_t port, uint16_t dat) {
				os_drivers_instr_outb(port, dat);
			}
		}
	}
}

#endif /* DRIVERS_INSTR_HPP */