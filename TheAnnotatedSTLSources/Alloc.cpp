#include "alloc.h"

//有关联链表等数据结构的东西, 看别人代码或者自己写代码的时候,要边写代码 ,一遍画图

namespace Jinl
{
	//��ʼ�����о�̬����
	char* Alloc::start_free = 0;
	char* Alloc::end_free = 0;
	size_t Alloc::heap_size = 0;

	// freelist是一个数组,里面存放了各种obj的指针
	// 指针数组
	Alloc::obj * volatile Alloc::freeList[Alloc::ENFreeLists::FREELISTS] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};

	void* Alloc::allocate(size_t bytes)
	{
		obj * volatile * myfreelist;
		obj * result;
		// 如果大于128字节,直接用malloc申请内存空间
		if (bytes > EMaxByte::MAXBYTE)
		{
			return malloc(bytes);	//返回申请内存的指针
		}
		// freeList第 index内存的位置
		myfreelist = freeList + FREELIST_INDEX(bytes);
		result = *myfreelist;
		// 如果是0,表示这个位置还没有分配内存,使用refill分配内存
		if (result == 0)
		{
			// 如果freelist在这个内存位置没有可用的分配内存,要重新填充free list
			void * r = refill(ROUND_UP(bytes));
			return r;
		}

		*myfreelist = result->next;	// freelist种某个index对应的内存位置指向下一个可用的位置
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
		char * chunk = chunkAlloc(n, nobjs);	//分配空间给内存池
		obj * volatile *myfreelist;	//指向obj指针 的指针
		obj * result;
		obj * current_obj, *next_obj;

		if (nobjs == 1) return(chunk);

		myfreelist = freeList + FREELIST_INDEX(n);

		//返回当前第一个可用的freelist的小内存指针
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
			// 把分配过来的连续内存做成内存池链表的形式
			else {
				current_obj->next = next_obj;
			}
			return result;
		}
	}


	char* Alloc::chunkAlloc(size_t size, size_t& nobjs)
	{
		char * result;
		size_t total_bytes = size*nobjs;	//在freelist这个位置,一次行分配20个大小的内存空间
		size_t bytes_left = end_free - start_free;	//从连续的内存空间里面进行分配

		//如果内存空间够的话,直接分配20个这么大的就行了,而且是连续的空间
		if (bytes_left >= total_bytes)
		{
			result = start_free;
			start_free = start_free + total_bytes;
			return result;
		}
		//�ڴ�ؿռ䲻�����е��ֽڷ��䣬�����㹻��Ӧһ�������ϵ�������
		// 如果不够20个那么大的空间,有多少内存分配多大的东东
		else if (bytes_left >= size) {
			nobjs = bytes_left / size;
			total_bytes = size*nobjs;
			result = start_free;
			start_free = start_free + total_bytes;

			return result;
		}
		// 如果连size都不够的话,就要重新分配内存了
		else {
			// 分配40倍的内存 + 后面这个不知道是什么
			size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
			//���ڴ���в���Ŀռ�����freeelist��
			// 让内存池剩余的一点东西还有价值
			// 把它就近分配到16个内存池种比较合适的位置
			// 这里有疑问
			if (bytes_left > 0)
			{
				obj * volatile *myfreelist = freeList + FREELIST_INDEX(bytes_left);
				((obj*)start_free)->next = *myfreelist;
				*myfreelist = (obj *)start_free;
			}
			//��heap�з���ռ���ڴ��
			//分配一个连续的内存空间
			start_free = (char*)malloc(bytes_to_get);
			//�������ʧ��
			// 如果分配不成功
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