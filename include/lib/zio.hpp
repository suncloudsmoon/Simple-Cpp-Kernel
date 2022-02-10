#ifndef LIB_ZIO_HPP
#define LIB_ZIO_HPP

#include <kernel/basic_io.hpp>
#include <lib/zstring.hpp>

namespace zl {

struct ostream {
	ostream& operator<<(bool b);
	ostream& operator<<(char c);
	ostream& operator<<(const char *str);
	ostream& operator<<(const zl::string &str);
};

constexpr char endl{'\n'};
extern ostream cout;

}  // namespace zl

#endif /* LIB_ZIO_HPP */