#ifndef BASIC_IO_HPP
#define BASIC_IO_HPP

// Due to some unknown error, I had to make this a define!
#define os_vga_ptr reinterpret_cast<volatile char *>(0xB8000)
constexpr unsigned int OS_SPACES_PER_TAB{4};

extern "C" {
enum OS_VGA_COLOR_TYPES {
  OS_VGA_COLOR_BLACK = 0,
  OS_VGA_COLOR_BLUE = 1,
  OS_VGA_COLOR_GREEN = 2,
  OS_VGA_COLOR_CYAN = 3,
  OS_VGA_COLOR_RED = 4,
  OS_VGA_COLOR_PURPLE = 5,
  OS_VGA_COLOR_BROWN = 6,
  OS_VGA_COLOR_GRAY = 7,
  OS_VGA_COLOR_DARK_GRAY = 8,
  OS_VGA_COLOR_LIGHT_BLUE = 9,
  OS_VGA_COLOR_LIGHT_GREEN = 10,
  OS_VGA_COLOR_LIGHT_CYAN = 11,
  OS_VGA_COLOR_LIGHT_RED = 12,
  OS_VGA_COLOR_LIGHT_PURPLE = 13,
  OS_VGA_COLOR_YELLOW = 14,
  OS_VGA_COLOR_WHITE = 15
};

constexpr unsigned int os_vga_coord_map(unsigned int x, unsigned int y,
                                        unsigned int num_rows) {
  return (y * num_rows) + x;
}

extern unsigned int os_vga_get_width();
extern unsigned int os_vga_get_height();
extern void os_vga_config_dimensions(unsigned int w, unsigned int h);
extern "C" void os_vga_cls_screen();

extern void os_putch(char c, unsigned char color_type);
}

#endif /* BASIC_IO_HPP */