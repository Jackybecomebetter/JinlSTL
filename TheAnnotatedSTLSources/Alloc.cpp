#include "alloc.h"

namespace Jinl
{
	//初始化类中静态变量
	char* Alloc::start_free = 0;
	char* Alloc::end_free = 0;
	size_t Alloc::heap_size = 0;
	Alloc::obj * volatile Alloc::freeList[Alloc::ENFreeLists::FREELISTS] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};

	void* Alloc::allocate(size_t bytes)
	{
		obj * volatile * myfreelist;
		obj * result;
		if (bytes > EMaxByte::MAXBYTE)
		{
			return malloc(bytes);
		}
		myfreelist = freeList + FREELIST_INDEX(bytes);
		result = *myfreelist;
		if (result == 0)
		{
			void * r = refill(ROUND_UP(bytes));
			return r;
		}
		*myfreelist = result->next;
		return result;
	}

	void Alloc::deallocate(void* p, size_t n)
	{
		obj* q = (obj*)p;
		obj* volatile *myfreelist;
		if (n > EMaxByte::MAXBYTE)
			free(p);
		myfreelist = freeList + FREELIST_INDEX(n);
		q->next = *myfreelist;
		*myfreelist = q;
	}
	void *Alloc::reallocate(void *ptr, size_t old_sz, size_t new_sz){
		deallocate(ptr, old_sz);
		ptr = allocate(new_sz);

		return ptr;
	}
	void * Alloc::refill(size_t n)
	{
		size_t nobjs = 20;
		char * chunk = chunkAlloc(n, nobjs);
		obj * volatile *myfreelist;
		obj * result;
		obj * current_obj, *next_obj;

		if (nobjs == 1) return(chunk);

		myfreelist = freeList + FREELIST_INDEX(n);

		result = (obj *)chunk;
		*myfreelist = next_obj = (obj *)(chunk + n);
		for (int i = 1;; i++)
		{
			current_obj = next_obj;
			next_obj = (obj *)((char*)next_obj + n);

			if (nobjs - 1 == i)
			{
				current_obj->next = 0;
				break;
			}
			else {
				current_obj->next = next_obj;
			}
			return result;
		}
	}


	char* Alloc::chunkAlloc(size_t size, size_t& nobjs)
	{
		char * result;
		size_t total_bytes = size*nobjs;
		size_t bytes_left = end_free - start_free;

		if (bytes_left >= total_bytes)
		{
			result = start_free;
			start_free = start_free + total_bytes;
			return result;
		}
		//内存池空间不够所有的字节分配，但是足够供应一个或以上的容量。
		else if (bytes_left >= size) {
			nobjs = bytes_left / size;
			total_bytes = size*nobjs;
			result = start_free;
			start_free = start_free + total_bytes;

			return result;
		}
		else {
			size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
			//将内存池中残余的空间分配给freeelist。
			if (bytes_left > 0)
			{
				obj * volatile *myfreelist = freeList + FREELIST_INDEX(bytes_left);
				((obj*)start_free)->next = *myfreelist;
				*myfreelist = (obj *)start_free;
			}
			//从heap中分配空间给内存池
			start_free = (char*)malloc(bytes_to_get);
			//如果分配失败
			if (start_free == 0)
			{
				obj* volatile *myfreelist, *p;
				for (int i = size; i <= MAXBYTE; i += Alloc::EAlign::ALIGN)
				{
					myfreelist = freeList + FREELIST_INDEX(i);
					p = *myfreelist;
					if (p != 0)
					{
						*myfreelist = p->next;
						start_free = (char*)p;
						end_free = start_free + i;

						return(chunkAlloc(size, nobjs)); 
					}
				}
				end_free = 0;
				
			}
			heap_size += bytes_to_get;
			end_free = start_free + bytes_to_get;

			return (chunkAlloc(size, nobjs));
		}

		
	}

}