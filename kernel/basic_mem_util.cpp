#include <kernel/basic_mem_util.hpp>

enum SIMPLE_REALLOC_CODES {
	OS_REALLOC_SUCCESS = 0,
	OS_REALLOC_NULL_MALLOC = -1,
	OS_REALLOC_BAD_MEMCPY = -2
};

// Static functions
static void* os_simple_alloc(size_t size, size_t *aligned_size_res);
static int os_simple_realloc(void *dest, void *src, size_t size, size_t *aligned_size_res);

template<typename T>
bool os_memcpy(T *dest, const T *src, size_t src_num_cpy) {
	if (!dest || !src) return false;
	
	for (size_t i = 0; i < src_num_cpy; i++) 
		if (dest[i]) dest[i] = src[i];
	
	return true;
}

template<>
bool os_memcpy(void *dest, const void *src, size_t src_num_cpy) {
	if (!dest || !src) return false;

	long *dest_lon = reinterpret_cast<long*>(dest);
	const long *src_lon = reinterpret_cast<const long*>(src);
	
	for (size_t i = 0; i < src_num_cpy; i++) 
		if (dest_lon[i]) dest_lon[i] = src_lon[i];
	
	return true;
}


struct ptr_info {
	void *ptr;
	size_t align;
};

// TODO: make a iterator for this struct
struct alloc_tracker {
	struct iterator {
		iterator(ptr_info *p) : loc(p) {}
		iterator operator++() {
			loc++;
			return *this;
		}
		bool operator!=(const iterator &other) {
			return loc != other.loc;
		}
		ptr_info* operator*() {
			return loc;
		}
	private:
		ptr_info *loc;
	};
	alloc_tracker(alloc_tracker *free_block_tracker) : is_usable(true) {
		free_tracker = free_block_tracker;	
		curr_index = 0;
		size = 10;
		list = static_cast<ptr_info**>(os_simple_alloc(size, &size));
		if (!list) {
			is_usable = false;
			size = 0;
			return;
		}
	}
	alloc_tracker(const alloc_tracker &other) = delete;
	alloc_tracker(alloc_tracker &&other) noexcept {
		list = other.list;
		curr_index = other.curr_index;
		size = other.size;
		is_usable = other.size;

		other.list = nullptr;
		other.curr_index = 0;
		other.size = 0;
		other.is_usable = 0;
	}

	void operator=(alloc_tracker &&other) {
		list = other.list;
		curr_index = other.curr_index;
		size = other.size;
		is_usable = other.size;

		other.list = nullptr;
		other.curr_index = 0;
		other.size = 0;
		other.is_usable = 0;
	}
	
	bool add(ptr_info *i) {
		if (!automatic_realloc(1)) return false;
		list[curr_index++] = i;
		return true;
	}

	bool add(void *ptr, size_t align) {
		size_t info_alignment{0};
		ptr_info *info = static_cast<ptr_info*>(os_simple_alloc(sizeof(ptr_info), &info_alignment));
		if (!info)
			return false;
		info->ptr = ptr;
		info->align = align;
		add(info);
		return true;	
	}

	bool remove(void *ptr) {
		if (!ptr)
			return false;
		for (size_t i = 0; i < size; i++) {
			ptr_info *p = list[i];
			if (ptr == p) {
				if (free_tracker) free_tracker->add(p);
				list[i] = nullptr;
				return true;
			} 
		}
		return false;
	}

	bool remove(size_t index) {
		if (index >= size)
			return false;
		ptr_info *p = list[index];
		if (!p)
			return false;
		if (free_tracker) free_tracker->add(p);
		list[index] = nullptr;	
		return true;	
	}

	// Getters and Setters
	ptr_info** get_list() {
		return list;
	}

	// Iterator functions
	iterator begin() {
		return iterator(list[0]);
	}
	iterator end() {
		return iterator(list[size]); // Should not access this
	}

private:
	bool automatic_realloc(size_t add) {
		if (curr_index + add >= size) {
			size_t old_size{size};
			size_t new_size{0};
			ptr_info **ptr{nullptr};
			auto ret = os_simple_realloc(ptr, list, (curr_index + add) * 2 * sizeof(ptr_info*), &new_size);
			
			if (ret == OS_REALLOC_BAD_MEMCPY) {
				is_usable = false;
				free_tracker->add(ptr, size);
				return false;
			} else if (ret == OS_REALLOC_NULL_MALLOC) {
				is_usable = false;
				return false;
			}
			size = new_size;
			
			if (free_tracker) free_tracker->add(list, size);
			list = ptr;
		}
		return true;
	}

	bool is_usable;
	alloc_tracker *free_tracker;
	ptr_info **list;
	size_t curr_index;
	size_t size;
};

// Global Variables
static size_t curr_stack_location;
static alloc_tracker allocated_block_tracker{nullptr};
static alloc_tracker free_block_tracker{nullptr};

/*
 * Initializes all the global variables
 */
extern "C" void os_mem_util_init() {
	curr_stack_location = 0x10000;
	free_block_tracker = alloc_tracker(nullptr);
	allocated_block_tracker = alloc_tracker(&free_block_tracker);
}

extern "C" void* os_alloc(size_t size, size_t *aligned_size_res) {
	if (!size || !aligned_size_res)
		return nullptr;
	void *ptr = os_simple_alloc(size, aligned_size_res);
	if (!ptr)
		return nullptr;
	if (!allocated_block_tracker.add(ptr, *aligned_size_res)) {
		free_block_tracker.add(ptr, *aligned_size_res);
		return nullptr;
	}
	return ptr;
}

extern "C" void* os_realloc(void *src, size_t size, size_t *aligned_size_res) {
	if (!src || !size || !aligned_size_res)
		return nullptr;
	void *new_ptr = os_alloc(size, aligned_size_res);
	if (!new_ptr)
		return nullptr;
	if (!os_memcpy(new_ptr, src, size)) {
		os_free(new_ptr);
		return nullptr;
	}
	return new_ptr;
}

extern "C" int os_free(void *ptr) {
	// automatically removes from the allocated block and adds it to the free block tracker
	return allocated_block_tracker.remove(ptr);
}

static void* os_simple_alloc(size_t size, size_t *aligned_size_res) {
	if (!size || !aligned_size_res) return nullptr;
	// Get the alignment
	*aligned_size_res = size / OS_ALLOC_ALIGNMENT + OS_ALLOC_ALIGNMENT;
	void *ptr = reinterpret_cast<void *>(curr_stack_location);
	curr_stack_location += *aligned_size_res;
	return ptr;
}

static int os_simple_realloc(void *dest, void *src, size_t size, size_t *aligned_size_res) {
	void *new_ptr = os_simple_alloc(size, aligned_size_res);
	if (!new_ptr)
		return OS_REALLOC_NULL_MALLOC;
	if (!os_memcpy(new_ptr, src, size)) {
		os_free(new_ptr);
		return OS_REALLOC_BAD_MEMCPY;
	}
	dest = new_ptr;
	return OS_REALLOC_SUCCESS;
}
