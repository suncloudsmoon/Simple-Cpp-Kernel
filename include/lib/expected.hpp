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