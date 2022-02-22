#ifndef LIB_STACK_HPP
#define LIB_STACK_HPP

#include <lib/growing_array.hpp>

namespace zl {
	template<typename T>
	class stack : public growing_array<T> {
		public:
			using growing_array<T>::growing_array;
			using growing_array<T>::operator=;

			inline T& front() {
				return *growing_array<T>::operator[](growing_array<T>::get_curr_len() - 1);
			}
			inline bool push(const T &other) {
				return growing_array<T>::add(other);
			}
			inline bool push(T &&other) {
				return growing_array<T>::add(other);
			}
			inline bool pop() {
				return growing_array<T>::remove(growing_array<T>::get_curr_len() - 1); 
			}
			inline bool empty() {
				return growing_array<T>::get_curr_len() == 0;
			}
	};
}

#endif /* LIB_STACK_HPP */