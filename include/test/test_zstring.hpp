/*
 * Copyright (c) 2022, suncloudsmoon and the Simple-Cpp-Kernel contributors.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef TEST_TEST_ZSTRING_HPP
#define TEST_TEST_ZSTRING_HPP

#include <lib/zassert.hpp>

#ifdef DEBUG
#include <lib/testunit.hpp>
#include <lib/zstring.hpp>

namespace zl {
	namespace test {
		class test_string : public zl::unit_test {
			public:
				bool test() override {
					zl::string test1("hello");
					zl::assert(test1.c_str() != nullptr, "[test_string, test1 failed] => nullptr");
					test1.append("hello");
					zl::assert(test1.length() == zl::strlen("hello") * 2, "test_string, test1 failed] => length is invalid!");

					zl::string test2;
					zl::assert(test2.c_str() != nullptr, "[test_string, test2 failed] => nullptr");
					test2.append("hi");
					zl::assert(test2.length() == zl::strlen("hi"), "[test_string, test2 failed] => length is invalid!");
					return true;
				}
		};
	}
}
#endif

#endif /* TEST_TEST_ZSTRING_HPP */