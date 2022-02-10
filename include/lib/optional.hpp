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