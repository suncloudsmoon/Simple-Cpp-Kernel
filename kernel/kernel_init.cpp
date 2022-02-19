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
	// zl::growing_array<char> arr;
	// if (arr) {
	// 	arr.add('a');
	// 	arr.add('b');
	// 	arr.add('b');
	// 	arr.add('b');
	// 	arr.add('b');
	// 	arr.add('b');
	// 	arr.add('b');
	// 	arr.add('b');
	// 	for (char c : arr)
	// 		zl::cout << c << zl::endl;	
	// }
	if (auto res = zl::malloc(10 * sizeof(char*))) {
		char **data = static_cast<char**>((*res).ptr);
		size_t index{0};
		for (char c = 'a'; c <= 'o'; c++)
			data[index++] = new char(c);
		for (int i = 0; i < index; i++)
			zl::cout << *data[i] << zl::endl;
	}
}

static void config_os() {
	zl::cout = zl::ostream();
	zl::cerr = zl::ostream();
	os_vga_config_dimensions(80, 25);
	os_vga_cls_screen();
    os::mem_init();
}