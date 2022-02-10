#include <lib/zmem.hpp>
#include <kernel/basic_mem_util.hpp>

namespace zl {
	void* malloc(size_t size) {
		size_t aligned_size{0};
		return os_alloc(size, &aligned_size);
	}
	void* realloc(void *ptr, size_t size, size_t *aligned_size_res) {
		return os_realloc(ptr, size, aligned_size_res);
	}
	bool free(void *ptr) {
		return static_cast<bool>(os_free(ptr));
	}
}

void* operator new(size_t size) {
	return zl::malloc(size);
}
void* operator new[](size_t size) {
	return zl::malloc(size);
}
void operator delete(void *ptr) {
	zl::free(ptr);
}
void operator delete[](void *ptr) {
	zl::free(ptr);
}
