#include <stddef.h>
#include <lib/zio.hpp>

namespace zl {
ostream& ostream::operator<<(bool b) {
  if (b)
    operator<<("true");
  else
    operator<<("false");
  return *this;
}
ostream& ostream::operator<<(char c) {
	os_putch(c, OS_VGA_COLOR_WHITE);
	return *this;
}
ostream& ostream::operator<<(const char* str) {
  size_t index{0};
  while (char c = str[index++]) { operator<<(c); }
	return *this;
}

ostream& ostream::operator<<(const zl::string &str) {
  return operator<<(str.c_str());
}

ostream cout;

}  // namespace zl
