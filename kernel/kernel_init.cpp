#include <kernel/kernel_init.hpp>
#include <lib/zio.hpp>
#include <lib/zmem.hpp>

// Testing
#include <lib/zstring.hpp>

static void config_os();

extern "C" void main() {
	config_os();

	char *a = new char[10];
	zl::cout << zl::strncpy(a, "hello\0", 5) << zl::endl;
	zl::cout << a << zl::endl;

	zl::string b = "hello";
	zl::cout << "\n\n------------string stuff------------\n";
	zl::cout << b << zl::endl;

	// struct example {
	// 	char a;
	// 	char b;
	// };
	// zl::unique_ptr<example> e{new example()};
	// e->a = 'a';
	// e->b = 'b';
	// zl::cout << e->a << zl::endl;
	// zl::cout << e->b << zl::endl;
}

static void config_os() {
	os_vga_config_dimensions(80, 25);
	os_vga_cls_screen();
    os_mem_util_init();
}