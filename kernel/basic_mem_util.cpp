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
	namespace mem {
		static void* simple_alloc(size_t size, size_t &aligned_size_res);
		static void* simple_realloc(void *src, size_t size, size_t &aligned_size_res, int &err_func);

		struct alloc_info {
			blk ptr;
			alloc_info *next = nullptr;
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

			bool add(const blk &ptr) {
				if (curr_item) {
					curr_item->ptr = ptr;
					size_t res;
					curr_item = curr_item->next = static_cast<alloc_info*>(simple_alloc(sizeof(alloc_info), res));
					len++;
					return true;
				}
				return false;
			}

			bool add(blk &&ptr) {
				if (curr_item) {
					curr_item->ptr = ptr;
					size_t res;
					curr_item = curr_item->next = static_cast<alloc_info*>(simple_alloc(sizeof(alloc_info), res));
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

		// Static objects
		static size_t curr_stack_loc;
		static alloc_tracker *mem_track;

		static void* simple_alloc(size_t size, size_t &aligned_size_res) {
			if (!size) return nullptr;
			void *ptr = reinterpret_cast<void*>(curr_stack_loc);
			aligned_size_res = zl::round(size + 8, static_cast<size_t>(16));
			curr_stack_loc += aligned_size_res;	
			return ptr;
		}

		static void* simple_realloc(void *src, size_t size, size_t &aligned_size_res, int &err_func) {
			if (!src || !size) {
				err_func = realloc_code::invalid_arg;
				return nullptr;
			}
			void *new_ptr = simple_alloc(size, aligned_size_res);
			if (!new_ptr) {
				err_func = realloc_code::null_malloc;
				return nullptr;
			}
			if (!zl::memcpy(new_ptr, src, size)) {
				if (mem_track) mem_track->add_to_free_list(blk(new_ptr, aligned_size_res));
				err_func = realloc_code::memcpy_fail;
				return nullptr;
			}
			err_func = realloc_code::success;
			return new_ptr;
		}

		/*
		* Initializes all static variables in this file.
		*/
		void init() {
			curr_stack_loc = dyn_alloc_loc;
			size_t res;
			void *ptr = simple_alloc(sizeof(alloc_tracker), res);
			mem_track = new(ptr) alloc_tracker(); // placement new for initializing the tracker
		}

		zl::expected<blk> alloc(size_t size) {
			if (!size)
				return {nullptr, "[os::mem::alloc(size_t) error] -> invalid size(0)!", alloc_code::invalid_size};
			size_t aligned_res = 0;
			void *ptr = simple_alloc(size, aligned_res);
			if (!ptr)
				return {nullptr, "[os::mem::alloc(size_t) error] -> no memory allocation due to unknown error!", alloc_code::mem_err};
			if (mem_track && !mem_track->add(blk(ptr, aligned_res)))
				return {nullptr, "[os::mem::alloc(size_t) error] -> unable to add allocated memory block to respective tracker!", alloc_code::tracker_fail};
			return {{ptr, aligned_res}};
		}

		zl::expected<blk> realloc(void *src, size_t size) {
			if (!src || !size)
				return {nullptr, "[os::mem::realloc(void*, size_t) error] -> invalid argument passed!", realloc_code::invalid_arg};
			auto new_blk = alloc(size);
			if (!new_blk)
				return new_blk;
			if (!zl::memcpy((*new_blk).ptr, src, size)) {
				free((*new_blk).ptr);
				return {nullptr, "[os::mem::realloc(void*, size_t) error] -> memcpy() failure!", realloc_code::memcpy_fail};
			}
			return new_blk;
		}
		/* automatically removes ptr from the alloc tracker and adds it to the free block tracker */
		bool free(void *ptr) {
			return (mem_track && ptr) ? mem_track->remove(ptr) : false;
		}
	}
}