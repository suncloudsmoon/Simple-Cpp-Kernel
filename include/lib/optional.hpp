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
			optional() : is_init(), data{} {}
			explicit optional(T &&a) : is_init(true), data(a) {}
			optional(const optional &other) : is_init(other.is_init), data(other.data) {}
			optional(optional &&other) : is_init(other.is_init), data(other.data) {}
			operator bool() { return is_init; }
			T& operator*() { return data; }
		private:
			bool is_init;
			T data;	
	};
	template<typename T>
	class optional<T*> {
		public:
			optional() : is_init(), data{} {}
			explicit optional(T &&a) : is_init(true), data(a) {}
			optional(const optional &other) = delete;
			optional(optional &&other) {
				is_init = other.is_init;
				data = other.data;

				other.is_init = false;
				other.data = nullptr;
			}
			operator bool() { return is_init; }
			T& operator*() { return data; }
		private:
			bool is_init;
			T data;	
	};
}

#endif /* LIB_OPTIONAL */