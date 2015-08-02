#ifndef _VECTOR_H_
#define _VECTOR_H_

#include "alloc.h"
#

namespace Jinl
{
	template <class T,class alloc = Alloc>
	class vector{
	public:
		typedef T           value_type;
		typedef value_type* pointer;
		typedef value_type& reference;
		typedef value_type* iterator;
		typedef	size_t      size_type;
		typedef ptrdiff_t   diffrence_type;

	protected:

		typedef alloc dataAllocator;
		iterator      start;           //目前使用空间的头
		iterator      finish;		   //目前使用空间的尾
		iterator      endOfStorage;    //目前可用空间的尾

		void insert_aux(iterator position, const T& x);
		void deallocate()
		{
			if (start)
				dataAllocator::deallocate(start, endOfStorage - start);
		}
		iterator allocate_and_fill(size_type n, const T& x)
		{
			iterator result = dataAllocator::allocate(n);
			uninitialized_fill_n(result, n, x);
			return result;
		}

		void fill_initialize(size_type n, const T& value)
		{
			start = allocate_and_fill(n, value);
			finish = start + n;
			endOfStorage = finish;
		}

	public:
		
		vector();
		vector(size_type n, const T& value);
		vector(int n, const T& value);
		vector(long n, const T& value);
		explicit vector(size_type n);

		~vector(){
			destroy(start, finish);
			deallocate();
		}
		void vector<T, Alloc>::insert(iterator position,size_type n,const T& x);
		reference front()
		{
			return *begin();
		}

		reference back()
		{
			return *(end() - 1);
		}

		void push_back(const T& x)
		{
			if (finish != endOfStorage)
			{
				construct(finish, n);
				++finish;
			}
			else {
				insert_aux(end(), x);
			}
		}
		void insert_aux(iterator position, const T& x)
		{
			if (finish != endOfStorage)
			{
				construct(finish, , *(finish - 1));
				++finish;
				T x_copy = x;
				copy_backward(position, finish - 2, finish - 1);
				*position = x;
			}
			else {
				const size_type oldSize = size();
				const size_type len = oldSize != 0 ? 1.5*oldSize : 1;

				iterator new_start = dataAllocator::allocate(len);
				iterator new_finish = new_start;
				try{
					new_finish = uninitialized_copy(start, position, new_start);
					construct(new_finish, x);
					++finish;
					new_finish = uninitialized_copy(position, finish, new_finish);
				}
				catch ()
				{
					destroy(new_start, new_finish);
					dataAllocator::deallocate(new_start, len);
					throw;
				}
				destroy(begin(), end());
				deallocate();

				start = new_start;
				finish = new_finish;
				endOfStorage = new_start + len;
			}

		}

		void pop_back()
		{
			--finish;
			destroy(finish);
		}
		
		iterator erase(iterator position){
			if (position + 1 != end())
				copy(position + 1, finish, position);
			--finish;
			destroy(finish);
			return position;
		}
		iterator erase(iterator first, iterator last)
		{
			iterator i = copy(last, finish, first);
			destroy(i, finish);
			finish = finish - (last - first);
			return first;
		}

		void resize(size_type ns, const T& x)
		{
			if (ns < size())
				erase(begin() + ns, end());
			else
				insert(end(), ns - size(), x);
		}

		void resize(size_type ns)
		{
			resize(ns, T());
		}

		void clear()
		{
			erase(begin(), end());
		}

		iterator begin()
		{
			return start;
		}

		iterator end()
		{
			return finish;
		}
		size_type size() const {
			return size_type(finish() - begin());
		}

		size_type capacity() const {
			return size_type(endOfStorage - begin());
		}

		bool empty() const {
			return begin() == end();
		}

		reference operator[](size_type n)
		{
			return *(begin() + n);
		}


		





	};
}


#endif