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

namespace zl {
	template<typename T>
	class expected {
		public:
			expected(const char *error_message = nullptr, int error_code = 0) : 
				val{}, err_message(error_message), err_code(error_code), is_err(error_message) {}
			expected(T& value, const char *error_message = nullptr, int error_code = 0) : 
				val(value), err_message(error_message), err_code(error_code), is_err(error_message) {}
			expected(T&& value, const char *error_message = nullptr, int error_code = 0) : 
				val(value), err_message(error_message), err_code(error_code), is_err(error_message) {}
			expected(const expected<T> &other) = default;
			expected(expected<T*> &&other) noexcept {
				val = other.val;
				err_message = other.err_message;
				err_code = other.err_code;
				is_err = other.is_err;

				other.val = nullptr;
				err_message = nullptr;
				err_code = 0;
				is_err = false;
			}
			expected& operator=(const expected<T> &other) = default;
			expected& operator=(expected<T> &&other) noexcept {
				val = other.val;
				err_code = other.err_code;
				err_message = other.err_message;
				is_err = other.is_err;

				err_code = 0;
				err_message = nullptr;
				is_err = false;
			}
			expected& operator=(expected<T*> &&other) noexcept {
				val = other.val;
				err_code = other.err_code;
				err_message = other.err_message;
				is_err = other.is_err;

				other.val = nullptr;
				err_code = 0;
				err_message = nullptr;
				is_err = false;
			}

			T& get_val() { return val; }
			int get_err_code() const { return err_code; }
			const char* get_err_message() const { return err_message; }
			bool is_error() { return is_err; }

			operator bool() { return !is_err; }
			T& operator*() { return val; }
			T* operator->() { return val; }
		private:
			T val;
			const char *err_message;
			int err_code;
			bool is_err;
	};
	template<typename T> using unexpected = expected<T>;
}

#endif /* LIB_EXPECTED_HPP */