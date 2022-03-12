#include <drivers/instr.hpp>

namespace os {
	namespace drivers {
		namespace instr {
			uint8_t inb(uint16_t port) {
#ifdef __GNUC__					
				uint8_t dat;
				asm volatile(
					"inb %1, %0"
					: "=a"(dat)
					: "d"(port)
				);
				return dat;
#else
#error "Your compiler is not supported for executing inw(port) function yet!"
#endif				
			}

			void outb(uint16_t port, uint8_t dat) {
#ifdef __GNUC__
				asm volatile(
					"outb %1, %0"
					:
					: "d"(port), "a"(dat)
				);
#else
#error "Your compiler is not supported for executing inw(port) function yet!"
#endif
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

			// Assembly trig functions
			double fsin(double x) {
#ifdef __GNUC__				
				double res;
				asm(
					"fldl %1;"
					"fsin;"
					"fstl %0;"
					: "=m"(res)
					: "m"(x)
				);
				return res;
#else
#error "Your compiler is not supported for executing fsin(x) function yet!"
#endif				
			}

			double fcos(double x) {
#ifdef __GNUC__				
				double res;
				asm(
					"fldl %1;"
					"fcos;"
					"fstl %0;"
					: "=m"(res)
					: "m"(x)
				);
				return res;
#else
#error "Your compiler is not supported for executing fcos(x) function yet!"
#endif
			}

			double fptan(double x) {
#ifdef __GNUC__				
				double res;
				asm(
					"fldl %1;"
					"fptan;"
					"fstl %0;"
					: "=m"(res)
					: "m"(x)
				);
				return res;
#else
#error "Your compiler is not supported for executing fptan(x) function yet!"
#endif
			}
			double fprem1(double dividend, double divisor) {
#ifdef __GNUC__				
				double res;
				asm(
					"fldl %1;"
					"fldl %2;"
					"fprem1;"
					"fstl %0;"
					: "=m"(res)
					: "m"(divisor), "m"(dividend)
				);
				return res;
#else
#error "Your compiler is not supported for executing fptan(x) function yet!"
#endif	
			}

		}
	}
}