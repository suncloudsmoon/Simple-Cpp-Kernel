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

#ifndef LIB_UNIQUE_PTR
#define LIB_UNIQUE_PTR

#include <stddef.h>

#include <lib/zassert.hpp>
#include <lib/zio.hpp>

namespace zl {
	template<typename T>
	class unique_ptr {
		public:
			explicit unique_ptr(T *pointer = nullptr) : ptr(pointer) {}
			unique_ptr(const unique_ptr &other) = delete;
			unique_ptr(unique_ptr &&other) noexcept {
				ptr = other.ptr;
				other.ptr = nullptr;
			}
			~unique_ptr() { delete ptr; }
			unique_ptr& operator=(const unique_ptr &other) = delete;
			unique_ptr& operator=(unique_ptr &&other) noexcept {
				delete ptr;
				ptr = other.ptr;
				other.ptr = nullptr;
				return *this;
			}
			operator bool() const { return get(); }
			
			T* get() const { return ptr; }
			T* release() {
				T *temp = ptr;
				ptr = nullptr;
				return temp;
			}
			void reset(T *pointer = nullptr) {
				delete ptr;
				ptr = pointer;
			}
			void swap(unique_ptr &other) {
				T *temp = ptr;
				ptr = other.ptr;
				other.ptr = ptr;
			}

			T& operator*() const {
				assert(get(), "[zl::unique_ptr<T>::operator*() error] -> trying to derefence a null pointer!");
				return *get();
			}
			T* operator->() const {
				assert(get(), "[zl::unique_ptr<T>::operator->() error] -> trying to access a null pointer!");
				return get();
			}
			
			template<typename Second>
			bool operator==(const unique_ptr<Second> &other) const { return get() == other.get(); }
			template<typename Second>
			bool operator!=(const unique_ptr<Second> &other) const { return get() != other.get(); }
			template<typename Second>
			bool operator<(const unique_ptr<Second> &other) const { return get() < other.get(); }
			template<typename Second>
			bool operator<=(const unique_ptr<Second> &other) const { return get() <= other.get(); }
			template<typename Second>
			bool operator>(const unique_ptr<Second> &other) const { return get() > other.get(); }
			template<typename Second>
			bool operator>=(const unique_ptr<Second> &other) const { return get() >= other.get(); }

			friend ostream& operator<<(ostream &out, const unique_ptr<T> &pointer) {
				out << reinterpret_cast<size_t>(pointer.get());
				return out;
			}
		private:
			T *ptr;
	};

	template<typename T>
	class unique_ptr<T[]> {
		public:
			explicit unique_ptr(T *pointer = nullptr) : ptr(pointer) {}
			unique_ptr(const unique_ptr &other) = delete;
			unique_ptr(unique_ptr &&other) noexcept {
				ptr = other.ptr;
				other.ptr = nullptr;
			}
			~unique_ptr() { delete[] ptr; }
			unique_ptr& operator=(const unique_ptr &other) = delete;
			unique_ptr& operator=(unique_ptr &&other) noexcept {
				delete[] ptr;
				ptr = other.ptr;
				other.ptr = nullptr;
				return *this;
			}
			operator bool() const { return get(); }
			
			T* get() const { return ptr; }
			T* release() { 
				T *temp = ptr;
				ptr = nullptr;
				return temp;
			}
			void reset(T *pointer = nullptr) {
				delete[] ptr;
				ptr = pointer;
			}
			void swap(unique_ptr &other) {
				T *temp = ptr;
				ptr = other.ptr;
				other.ptr = ptr;
			}
			T& operator[](size_t index) const {
				assert(get(), "[zl::unique_ptr<T[]>::operator[](size_t) error] -> trying to access an element inside null array!");
				return get()[index];
			}
			
			template<typename Second>
			bool operator==(const unique_ptr<Second> &other) const { return get() == other.get(); }
			template<typename Second>
			bool operator!=(const unique_ptr<Second> &other) const { return get() != other.get(); }
			template<typename Second>
			bool operator<(const unique_ptr<Second> &other) const { return get() < other.get(); }
			template<typename Second>
			bool operator<=(const unique_ptr<Second> &other) const { return get() <= other.get(); }
			template<typename Second>
			bool operator>(const unique_ptr<Second> &other) const { return get() > other.get(); }
			template<typename Second>
			bool operator>=(const unique_ptr<Second> &other) const { return get() >= other.get(); }

			friend ostream& operator<<(ostream &out, const unique_ptr<T[]> &pointer) {
				out << reinterpret_cast<size_t>(pointer.get());
				return out;
			}
		private:
			T *ptr;
	};
}

#endif /* LIB_UNIQUE_PTR */