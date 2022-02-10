#ifndef LIB_UNIQUE_PTR
#define LIB_UNIQUE_PTR

namespace zl {
	template<typename T>
	class unique_ptr {
		public:
			unique_ptr() : ptr(nullptr) {}
			explicit unique_ptr(T* init) : ptr(init) {}
			unique_ptr(const unique_ptr &other) = delete;
			unique_ptr(unique_ptr &&other) {
				ptr = other.ptr;
				other.ptr = nullptr;
			}
			~unique_ptr() { delete ptr; }
			unique_ptr& operator=(const unique_ptr &other) = delete;
			unique_ptr& operator=(unique_ptr &&other) {
				delete ptr;
				ptr = other.ptr;
				other.ptr = nullptr;
				return *this;
			}
			T* operator*() { return ptr; }
			T* operator->() { return ptr; }
			operator bool() { return ptr; }
		private:
			T *ptr;
	};
}

#endif /* LIB_UNIQUE_PTR */