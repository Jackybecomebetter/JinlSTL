#ifndef _ALLOC_H_
#define _ALLOC_H_

#include "cstdlib"
namespace Jinl
{
	class Alloc{
	private:
		enum EAlign{ ALIGN = 8 };	//小型区块的上调边界
		enum EMaxByte{MAXBYTE = 128};	// 小型区块上届
		enum ENFreeLists{ FREELISTS = (EMaxByte::MAXBYTE/EAlign::ALIGN) };	//小型区块的类型数目,共16种
		enum ENOBJS{NOBJS = 20};

	private:

		// 这里看不太懂obj free-list的union数据结构
		union obj
		{
			union obj* next;
			char client[1];
		};

		// freelist是一个数组,里面存放了16种小内存类型
		static obj * volatile freeList[ENFreeLists::FREELISTS];
	private:
		static char* start_free;
		static char* end_free;
		static size_t heap_size;

		// 向上取整,例如7 变成8
		static size_t ROUND_UP(size_t bytes)
		{
			return ((bytes + EAlign::ALIGN - 1) & ~(EAlign::ALIGN - 1));
		}
		// free list有16种, 分别是 8,16,24,32,40,48,56,64,72,80,88,96,104,112,120,128
		// 这里根据申请字节的大小确定是申请那个free list,向上申请,就是如果是34字节,就申请40大小的那个free list 的index 下标,就是第五个
		static size_t FREELIST_INDEX(size_t bytes)
		{
			return (((bytes)+EAlign::ALIGN - 1) / EAlign::ALIGN - 1);
		}

		static void* refill(size_t n);
		static char* chunkAlloc(size_t n, size_t& nobjs);
	public:
		static void * allocate(size_t n);
		static void  deallocate(void* p, size_t n);
		static void * reallocate(void* p, size_t old_sz, size_t new_sz);
	};
}



#endif