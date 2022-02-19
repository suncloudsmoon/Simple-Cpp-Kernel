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

#include <kernel/basic_mem_util.hpp>
#include <lib/zmem.hpp>
#include <lib/zassert.hpp>
#include <lib/zmath.hpp>

namespace os {
	// Static functions
	static void* simple_alloc(size_t size, size_t &aligned_size_res);
	static void* simple_realloc(void *src, size_t size, size_t &aligned_size_res, int &err_func);

	struct alloc_info {
		blk ptr;
		alloc_info *next;
	};

	struct alloc_tracker {
		alloc_tracker() {
			size_t res;
			list = static_cast<alloc_info*>(simple_alloc(sizeof(alloc_info), res));
			curr_item = list;
			len = 1;

			free_list = static_cast<alloc_info*>(simple_alloc(sizeof(alloc_tracker), res));
			free_curr_item = free_list;
			free_len = 1;
		}
		~alloc_tracker() = default;

		bool add(const blk &ptr) {
			if (curr_item) {
				curr_item->ptr = ptr;
				size_t res;
				curr_item->next = static_cast<alloc_info*>(simple_alloc(sizeof(alloc_info), res));
				curr_item = list->next;
				len++;
				return true;
			}
			return false;
		}

		bool add(blk &&ptr) {
			if (curr_item) {
				curr_item->ptr = ptr;
				size_t res;
				curr_item->next = static_cast<alloc_info*>(simple_alloc(sizeof(alloc_info), res));
				curr_item = list->next;
				len++;
				return true;
			}
			return false;
		}

		bool add_to_free_list(const blk &ptr) {
			if (curr_item) {
				free_curr_item->ptr = ptr;
				size_t res;
				free_curr_item->next = static_cast<alloc_info*>(simple_alloc(sizeof(alloc_info), res));
				free_curr_item = free_list->next;
				free_len++;
				return true;
			}
			return false;
		}

		bool add_to_free_list(blk &&ptr) {
			if (curr_item) {
				free_curr_item->ptr = ptr;
				size_t res;
				free_curr_item->next = static_cast<alloc_info*>(simple_alloc(sizeof(alloc_info), res));
				free_curr_item = free_list->next;
				free_len++;
				return true;
			}
			return false;
		}

		bool remove(void *ptr) {
			alloc_info *prev = nullptr;
			alloc_info *ch = list;
			while (ch) {
				if (ch->ptr.ptr == ptr) {
					if (prev) {
						prev->next = ch->next;
					} else {
						list = ch->next;
					}
					len--;
					free_curr_item->ptr = zl::move(ch->ptr);
					size_t res;
					free_curr_item = free_curr_item->next = static_cast<alloc_info*>(simple_alloc(sizeof(alloc_info), res));
					free_len++;
					return true;
				}
				prev = ch;
				ch = ch->next;
			}
			return false;
		}

		private:
			alloc_info *list;
			alloc_info *curr_item;
			size_t len;

			alloc_info *free_list;
			alloc_info *free_curr_item;
			size_t free_len;
	};

	// Global Variables
	static size_t curr_stack_location;
	static alloc_tracker *tracker;

	static void* simple_alloc(size_t size, size_t &aligned_size_res) {
		if (!size) return nullptr;
		void *ptr = reinterpret_cast<void*>(curr_stack_location);
		aligned_size_res = zl::round(size + 8, static_cast<size_t>(16));
		curr_stack_location += aligned_size_res;	
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
			if (tracker) tracker->add_to_free_list(blk(new_ptr, aligned_size_res));
			err_func = REALLOC_MEMCPY_FAILURE;
			return nullptr;
		}
		return new_ptr;
	}

	/*
	* Initializes all the global variables
	*/
	void mem_init() {
		curr_stack_location = 0x9770;
		size_t res;
		void *ptr = simple_alloc(sizeof(alloc_tracker), res);
		tracker = new(ptr) alloc_tracker(); // placement new for our tracker
	}

	zl::expected<blk> alloc(size_t size) {
		if (!size)
			return {nullptr, "[os::alloc() error] -> Invalid size(0)!", ALLOC_INVALID_SIZE};
		size_t aligned_res{0};
		void *ptr = simple_alloc(size, aligned_res);
		if (!ptr)
			return {nullptr, "[os::alloc() error] -> No memory allocation due to unknown error!", ALLOC_MEM_ERR};
		if (tracker && !tracker->add(blk(ptr, aligned_res))) {
			return {nullptr, "[os::alloc() error] -> Unable to add allocated memory block to respective tracker!", ALLOC_TRACKER_FAILURE};
		}
		return {{ptr, aligned_res}};
	}

	zl::expected<blk> realloc(void *src, size_t size) {
		if (!src || !size)
			return {nullptr, "[os::realloc() error] -> invalid argument passed!", REALLOC_INVALID_ARG};
		auto new_blk = alloc(size);
		if (!new_blk)
			return new_blk;
		if (!zl::memcpy((*new_blk).ptr, src, size)) {
			free((*new_blk).ptr);
			return {nullptr, "[os::realloc() error] -> memcpy() failure!", REALLOC_MEMCPY_FAILURE};
		}
		return new_blk;
	}

	bool free(void *ptr) {
		/* automatically removes from the allocated block and adds it to the free block tracker */
		return (tracker && ptr) ? tracker->remove(ptr) : false;
	}
}