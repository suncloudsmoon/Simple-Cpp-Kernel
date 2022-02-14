#include <kernel/kernel_init.hpp>
#include <lib/zio.hpp>
#include <lib/zmem.hpp>
#include <lib/zstring.hpp>

// Testing
#include <lib/growing_array.hpp>

static void config_os();
static zl::expected<char> something();


extern "C" void main() {
	config_os();
	// if (auto res = zl::malloc(5 * sizeof(char*))) {
	// 	char **data = static_cast<char**>((*res).ptr);
	// 	data[0] = new char('a');
	// 	data[1] = new char('b');
	// 	zl::cout << *data[0] << zl::endl;
	// 	zl::cout << *data[1] << zl::endl;

	// 	// zl::cout << "Size: " << (char) ((1 * sizeof(char*)) + '0') << zl::endl;
	// 	// zl::cout << "Size: " << (char) (( ((2) * sizeof(char*)) - (1 * sizeof(char*)) ) + '0') << zl::endl;
	// }
	zl::cout = zl::ostream();
	zl::cout << "hello world!" << zl::endl;
}

static void config_os() {
	os_vga_config_dimensions(80, 25);
	os_vga_cls_screen();
    os::mem_init();
}