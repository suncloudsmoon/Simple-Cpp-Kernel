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

#ifndef LIB_OPTIONAL
#define LIB_OPTIONAL

namespace zl {
	template<typename T>
	class optional {
		public:
			optional() = default;
			explicit optional(T &&a) : data(a), is_init(true) {}
			optional(const optional &other) : data(other.data), is_init(other.is_init) {}
			optional(optional &&other) : data(other.data), is_init(other.is_init) {}
			operator bool() { return is_init; }
			T& operator*() { return data; }
		private:
			T data{};
			bool is_init{};
	};
	template<typename T>
	class optional<T*> {
		public:
			optional() = default;
			explicit optional(T &&a) : data(a), is_init(true) {}
			optional(const optional &other) = delete;
			optional(optional &&other) {
				data = other.data;
				is_init = other.is_init;

				other.data = nullptr;
				other.is_init = false;
			}
			operator bool() { return is_init; }
			T& operator*() { return data; }
		private:
			T data{};
			bool is_init{};
	};
}

#endif /* LIB_OPTIONAL */