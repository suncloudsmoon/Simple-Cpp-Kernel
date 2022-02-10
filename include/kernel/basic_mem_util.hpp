#ifndef KERNEL_BASIC_MEM_UTIL
#define KERNEL_BASIC_MEM_UTIL

#include <stddef.h>

constexpr size_t OS_ALLOC_ALIGNMENT{8};

extern "C" void os_mem_util_init();
extern "C" void* os_alloc(size_t size, size_t *aligned_size_res);
extern "C" void* os_realloc(void *src, size_t size, size_t *aligned_size_res);
extern "C" int os_free(void *ptr);

#endif /* KERNEL_BASIC_MEM_UTIL */