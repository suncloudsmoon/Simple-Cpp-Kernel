#ifndef LIB_ZMATH_HPP
#define LIB_ZMATH_HPP

namespace zl {
	template<typename T>
	inline T round(T val, T nearest) {
		return (val / nearest) * nearest + nearest;
	}
}

#endif /* LIB_ZMATH_HPP */