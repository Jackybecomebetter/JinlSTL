#ifndef _ALLOC_H_
#define _ALLOC_H_

#include "cstdlib"
namespace Jinl
{
	class Alloc{
	private:
		enum EAlign{ ALIGN = 8 };
		enum EMaxByte{MAXBYTE = 128};
		enum ENFreeLists{ FREELISTS = (EMaxByte::MAXBYTE/EAlign::ALIGN) };
		enum ENOBJS{NOBJS = 20};

	private:

		union obj
		{
			union obj* next;
			char client[1];
		};

		static obj * volatile freeList[ENFreeLists::FREELISTS];
	private:
		static char* start_free;
		static char* end_free;
		static size_t heap_size;

		static size_t ROUND_UP(size_t bytes)
		{
			return ((bytes + EAlign::ALIGN - 1) & ~(EAlign::ALIGN - 1));
		}
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