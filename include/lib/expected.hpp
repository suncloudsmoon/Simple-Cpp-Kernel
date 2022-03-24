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

#ifndef LIB_EXPECTED_HPP
#define LIB_EXPECTED_HPP

#include <lib/zassert.hpp>

namespace zl {
	template<typename T>
	class expected {
		public:
			constexpr expected(const char *error_message = nullptr, int error_code = 0) : 
				val{}, err_message(error_message), err_code(error_code) {}
			constexpr expected(T&& value, const char *error_message = nullptr, int error_code = 0) : 
				val(value), err_message(error_message), err_code(error_code) {}
			constexpr operator bool() const { return !is_error(); }

			constexpr T& get_val() { return val; }
			constexpr int get_err_code() const { return err_code; }
			constexpr const char* get_err_message() const { return err_message; }
			constexpr bool is_error() const { return err_message; }

			constexpr T& operator*() { return get_val(); }
		private:
			T val;
			const char *err_message;
			int err_code;
	};

	template<typename T>
	class expected<T*> {
		public:
			constexpr expected(const char *error_message = nullptr, int error_code = 0) : 
				val{}, err_message(error_message), err_code(error_code) {}
			constexpr expected(T& value, const char *error_message = nullptr, int error_code = 0) : 
				val(value), err_message(error_message), err_code(error_code) {}
			constexpr expected(T&& value, const char *error_message = nullptr, int error_code = 0) : 
				val(value), err_message(error_message), err_code(error_code) {}
			constexpr expected(const expected<T> &other) = delete;
			constexpr expected(expected &&other) noexcept {
				val = other.val;
				err_message = other.err_message;
				err_code = other.err_code;

				other.val = nullptr;
				err_message = nullptr;
				err_code = 0;
			}
			constexpr expected& operator=(const expected &other) = delete;
			constexpr expected& operator=(expected &&other) noexcept {
				val = other.val;
				err_code = other.err_code;
				err_message = other.err_message;

				other.val = nullptr;
				err_code = 0;
				err_message = nullptr;

				return *this;
			}
			constexpr operator bool() const { return !is_error(); }

			constexpr T& get_val() const { return *val; }
			constexpr int get_err_code() const { return err_code; }
			constexpr const char* get_err_message() const { return err_message; }
			constexpr bool is_error() const { return err_message; }

			constexpr T& operator*() const {
				assert(val, "[zl::expected::operator*() error] -> val is null!");
				return *val;
			}
			constexpr T* operator->() const {
				assert(val, "[zl::expected::operator->() error] -> val is null!");
				return val;
			}
		private:
			T val;
			const char *err_message;
			int err_code;
	};

	template<typename T> using unexpected = expected<T>;
}

#endif /* LIB_EXPECTED_HPP */