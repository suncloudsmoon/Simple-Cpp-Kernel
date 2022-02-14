#include <kernel/basic_mem_util.hpp>
#include <lib/zmem.hpp>
#include <lib/assert.hpp>

namespace os {
	// Static functions
	static void* simple_alloc(size_t size, size_t &aligned_size_res);
	static void* simple_realloc(void *src, size_t size, size_t &aligned_size_res, int &err_func);

	struct ptr_info {
		void *ptr;
		size_t align;
	};

	// TODO: make a iterator for this struct
	struct alloc_tracker {
		alloc_tracker(alloc_tracker *free_block_tracker) : 
				curr_index(0), size(10), is_usable(true), free_tracker(free_block_tracker) {
			list = static_cast<ptr_info**>(simple_alloc(size, size));
			if (!list) {
				is_usable = false;
				size = 0;
				if (free_tracker) free_tracker->add(list, size);
			}
		}
		alloc_tracker(const alloc_tracker &other) = delete;
		alloc_tracker(alloc_tracker &&other) noexcept {
			list = other.list;
			curr_index = other.curr_index;
			size = other.size;
			is_usable = other.is_usable;
			free_tracker = other.free_tracker;

			other.list = nullptr;
			other.curr_index = 0;
			other.size = 0;
			other.is_usable = false;
			other.free_tracker = nullptr;
		}

		bool operator=(alloc_tracker &&other) noexcept {
			if (!is_usable) {
				list = other.list;
				curr_index = other.curr_index;
				size = other.size;
				is_usable = other.is_usable;
				free_tracker = other.free_tracker;

				other.list = nullptr;
				other.curr_index = 0;
				other.size = 0;
				other.is_usable = false;
				other.free_tracker = nullptr;

				return true;
			}
			return false;
		}
		
		bool add(ptr_info *i) {
			if (!automatic_realloc(1))
				return false;
			list[curr_index++] = i;
			return true;
		}

		bool add(void *ptr, size_t align) {
			size_t info_alignment{0};
			ptr_info *info = static_cast<ptr_info*>(simple_alloc(sizeof(ptr_info), info_alignment));
			if (!info)
				return false;
			info->ptr = ptr;
			info->align = align;
			if (!add(info)) {
				if (free_tracker) free_tracker->add(info);
				return false;
			}
			return true;
		}

		bool remove(void *ptr) {
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

		// Getters
		ptr_info** get_list() {
			return list;
		}
	protected:
		bool automatic_realloc(size_t add) {
			if (curr_index + add >= size) {
				size_t old_size{size};
				
				size_t new_size{0};
				int err_code{0};
				ptr_info **new_ptr = 
				static_cast<ptr_info**>
				(simple_realloc(list, (curr_index + add) * 2 * sizeof(ptr_info*), new_size, err_code));
				
				switch (err_code) {
					[[likely]] case REALLOC_SUCCESS:
						break;
					[[unlikely]] case REALLOC_INVALID_ARG:
						break;
					[[likely]] case REALLOC_NULL_MALLOC: {
						is_usable = false;
						return false;
					}
					[[unlikely]] case REALLOC_MEMCPY_FAILURE: {
						is_usable = false;
						free_tracker->add(new_ptr, new_size);
						return false;
					}
				}
				size = new_size;
				if (free_tracker) free_tracker->add(list, old_size);
				list = new_ptr;
			}
			return true;
		}
	private:
		bool is_usable;
		ptr_info **list;
		size_t curr_index;
		size_t size;
		alloc_tracker *free_tracker;
	};

	// Global Variables
	static size_t curr_stack_location;
	static alloc_tracker allocated_block_tracker{nullptr};
	static alloc_tracker free_block_tracker{nullptr};

	static void* simple_alloc(size_t size, size_t &aligned_size_res) {
		if (!size) 
			return nullptr;
		void *ptr = reinterpret_cast<void*>(curr_stack_location);
		aligned_size_res = ((size / ALLOC_ALIGNMENT) * ALLOC_ALIGNMENT) + ALLOC_ALIGNMENT;
		curr_stack_location += aligned_size_res;
#ifdef DEBUG
	zl::cout << "Aligned Size Res: " << (char) (aligned_size_res + '0') << zl::endl;
#endif		
		return ptr;
	}

	static void* simple_realloc(void *src, size_t size, size_t &aligned_size_res, int &err_func) {
		if (!src || !size) {
			err_func = REALLOC_INVALID_ARG;
			return nullptr;
		}
		void *new_ptr = simple_alloc(size, aligned_size_res);
		if (!new_ptr) {
			err_func = REALLOC_NULL_MALLOC;
			return nullptr;
		}
		if (!zl::memcpy(new_ptr, src, size)) {
			free_block_tracker.add(new_ptr, aligned_size_res);
			return nullptr;
		}
		return new_ptr;
	}

	/*
	* Initializes all the global variables
	*/
	void mem_init() {
		curr_stack_location = 0x10000;
		free_block_tracker = alloc_tracker(nullptr);
		allocated_block_tracker = alloc_tracker(&free_block_tracker);
	}

	zl::expected<blk> alloc(size_t size) {
		if (!size)
			return {nullptr, "Passed invalid size(0) to os_alloc()!", ALLOC_INVALID_SIZE};
		size_t aligned_res{0};
		void *ptr = simple_alloc(size, aligned_res);
		if (!ptr)
			return {nullptr, "Unable to allocate memory due to unknown error!", ALLOC_MEM_ERR};
		
		if (!allocated_block_tracker.add(ptr, aligned_res)) {
			free_block_tracker.add(ptr, aligned_res);
			return {nullptr, "[os_alloc() error] -> unable to add allocated memory block to respective tracker!", ALLOC_TRACKER_FAILURE};
		}
		return {{ptr, aligned_res}};
	}

	zl::expected<blk> realloc(void *src, size_t size) {
		if (!src || !size)
			return {nullptr, "[os_realloc() error] -> invalid argument passed!", REALLOC_INVALID_ARG};
		auto new_blk = alloc(size);
		if (!new_blk)
			return new_blk;
		if (!zl::memcpy((*new_blk).ptr, src, size)) {
			free((*new_blk).ptr);
			return {nullptr, "[os_realloc() error] -> memcpy() failure!", REALLOC_MEMCPY_FAILURE};
		}
		return new_blk;
	}

	bool free(void *ptr) {
		/* automatically removes from the allocated block and adds it to the free block tracker */
		return allocated_block_tracker.remove(ptr);
	}
}