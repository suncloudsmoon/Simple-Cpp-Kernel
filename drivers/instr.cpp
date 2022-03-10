#include <drivers/instr.hpp>

namespace os {
	namespace drivers {
		namespace instr {
			uint8_t inb(uint16_t port) {
				uint8_t dat;
				asm volatile(
					"inb %1, %0"
					: "=a"(dat)
					: "d"(port)
				);
				return dat;
			}

			void outb(uint16_t port, uint8_t dat) {
				asm volatile(
					"outb %1, %0"
					:
					: "d"(port), "a"(dat)
				);
			}

			uint16_t inw(uint16_t port) {
#ifdef __GNUC__				
				uint16_t dat;
				asm volatile (
					"inw %1, %0"
					: "=a"(dat)
					: "d"(port)
				);
				return dat;
#else
#error "Your compiler is not supported for executing inw(port) function yet!"
#endif
			}

			void outw(uint16_t port, uint16_t dat) {
#ifdef __GNUC__				
				asm volatile(
					"outw %0, %1"
					:
					: "a"(dat), "d"(port)
				);
#else
#error "Your compiler is not supported for executing outw(port) function yet!"
#endif				
			}
		}
	}
}