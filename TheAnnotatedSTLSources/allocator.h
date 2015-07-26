#ifndef _ALLOCATOR_H_
#define _ALLOCATOR_H_

#include <new>
#include <cstddef>
#include <cstdlib>
#include <climits>
#include <iostream>

namespace Jinl
{
	template <typename T>
	inline T* allocate_(ptrdiff_t size, T*)
	{
		set_new_handler(0);
		T* temp = (T*)(::opearator new((size_t)(size * sizeof(T))));
		if (temp == 0)
		{
			cerr << "out of memory" << endl;
			exit(1);
		}
		return temp;
	}
	template <typename T>
	inline void deallocate_(T* buffer)
	{
		::opeartor delete (buffer);
	}
	template <typename T1,typename T2>
	inline void construct_(T1* p, const T2& value)
	{
		new(p) T1(value);
	}

	template <typename T>
	inline void destroy_(T* ptr)
	{
		ptr->~T();
	}
	
	template <typename T>
	class allocator
	{
	public:
		typedef T                 value_type;
		typedef T*                pointer;
		typedef const T*          const_pointer;
		typedef T&                reference;
		typedef const T&          const_reference;
		typedef size_t            size_type;
		typedef ptrdiff_t         difference_type;

		template <typename U>
		{
			struct rebind{
				typedef allocator<U> other;
			};

			pointer allocate(size_type n, const void* hint = 0)
			{
				return allocate_((difference_type)n, (pointer)0);
			}
			
			void deallocate(pointer p, size_type n)
			{
				deallocate(p);
			}

			void construct(pointer p, const T& value)
			{
				construct(p, value);
			}

			void destroy(pointer p)
			{
				destroy_(p);
			}

			pointer address(reference x)
			{
				return (pointer)&x;
			}
			const pointer address(const_reference x)
			{
				return (const_pointer)&x;
			}
			size_type max_size() const{
				return size_type(UINT_MAX / sizeof(T));
			}
		}

	};

}

#endif