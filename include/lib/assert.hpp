#ifndef LIB_ASSERT_HPP
#define LIB_ASSERT_HPP

#define DEBUG

#ifdef DEBUG
#include <lib/zio.hpp>
#else
#define RELEASE
#endif

namespace zl {
	inline constexpr void assert(bool condition, const char *message = "") {
#ifdef DEBUG		
		if (!condition)
			zl::cout << "Assertion Failed: " << message << zl::endl;
#else
		(void) 0;
#endif		
	}
}

#endif /* LIB_ASSERT_HPP */