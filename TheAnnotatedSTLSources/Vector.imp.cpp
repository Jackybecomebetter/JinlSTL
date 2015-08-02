#include "Vector.h"

namespace Jinl
{
	template<class T,class Alloc>
	void vector<T, Alloc>::insert(iterator position, size_type n, const T& x)
	{
		if (n != 0)
		{
			//有空间容纳新增的元素
			if (size_type(endOfStorage - finish) >= 0)
			{
				T x_copy = x;
				const size_type elems_after = finish - position;
				iterator oldFinish = finish;
				if (elems_after > n)
				{
					uninitialized_copy(finish - n, finish, finish);
					finish += n;
					copy_backward(position, oldFinish - n, oldFinish);
					fill(position, position + n, x_copy);
				}
				else{
					uninitialized_fill_n(finish, n - elems_after, x_copy);
					finish += n - elems_after;
					uninitialized_copy(position, oldFinish, finish);
					finish += elems_after;
					fill(position, oldFinish, x_copy);
				}
			}
			else{
				//备用空间小于新增元素个数，空间大小不足，必须分配额外的空间
				const size_type oldSize = size();
				const size_type newSize = oldSize + max(oldSize, n);

				iterator newStart = dataAllocator::allocate(newSize);
				iterator newFinish = newStart;
				try{
					newFinish = uninitialized_copy(start, finish, newStart);
					newFinish = uninitialized_fill_n(newFinish, n, x);
					newFinish = uninitialized_copy(position, finish, newFinish);
				}
				catch ()
				{
					destroy(newStart, newFinish);
					dataAllocator::deallocate(newStart, newSize);
					throw;
				}
				destroy(start, finish);
				deallocate();

				start = newStart;
				finish = newFinish;
				endOfStorage = start + newSize;
			}
		}
	}
}