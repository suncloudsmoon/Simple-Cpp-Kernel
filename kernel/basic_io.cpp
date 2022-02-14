#include <kernel/basic_io.hpp>

unsigned int x{};
unsigned int y{};

unsigned int width{};
unsigned int height{};

extern "C" unsigned int os_vga_get_width() { return width; }
extern "C" unsigned int os_vga_get_height() { return height; }
extern "C" void os_vga_config_dimensions(unsigned int w, unsigned int h) {
  width = w;
  height = h;
}
extern "C" void os_vga_cls_screen() {
  for (unsigned int y = 0; y < height; y++) {
    for (unsigned int x = 0; x < width * 2; x += 2) {
      auto pos = os_vga_coord_map(x, y, width);
      os_vga_ptr[pos] = '\0';
      os_vga_ptr[pos + 1] = OS_VGA_COLOR_WHITE;
    }
  }
}
extern "C" void os_putch(char c, unsigned char color_type) {
  if (c == '\n') {
    y++;
    x = 0;
    return;
  } else if (c == '\t') {
    x += OS_SPACES_PER_TAB;
    return;
  }
  if (x >= width * 2) {
    y++;
    x = 0;
  }
  if (y >= height)
    return;

  auto pos = os_vga_coord_map(x, y, width * 2);
  os_vga_ptr[pos] = c;
  os_vga_ptr[pos + 1] = color_type;
  x += 2;
}
