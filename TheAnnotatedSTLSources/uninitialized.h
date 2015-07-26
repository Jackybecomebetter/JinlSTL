#ifndef _UNINITIALIZED_H_
#define _UNINITIALIZED_H_

#include "construct.h"
#include "string.h"
namespace Jinl
{
	/*作用：在一块未初始化的内存上，调用copy constructor,
	 *在该范围内产生x的复制品。*/
	template <typename ForwardIterator,typename Size,typename T>
	inline ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T& x)
	{
		return _uninitialized_fill_n(first, n, x, value_type(first));
	}
	template<typename ForwardIterator, typename Size, class T, class T1>
	inline ForwardIterator _uninitialized_fill_n(ForwardIterator first, Size n, const T& x, T1*)
	{
		typedef typename _type_traits<T1>::is_POD_type is_POD;
		return _uninitialized_fill_n_aux(first, n, x, is_POD());
	}

	template <typename ForwardIterator,typename Size,typename T>
	inline ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, _true_type)
	{
		return fill_n(first, n, x);
	}

	template <typename ForwardIterator,typename Size,typename T>
	ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, _false_type)
	{
		ForwardIterator curr = first;
		for (; n > 0; --n, ++curr)
			construct(&*curr, x);
		return curr;
	}


	/*如果作为输出目的地的范围内的每一个迭代器都指向未初始化的区域，
	 *则调用copy constructor,给输入来源的范围内的每一个对象产生一份复制品*/

	template <typename ForwardIterator,typename InputIterator>
	inline ForwardIterator 
		uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result)
	{
		return _uninitialized_copy(first, last, result, value_type(result));
	}

	template <typename ForwardIterator, typename InputIterator,typename T>
	inline ForwardIterator
		_uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result, T*)
	{
		typedef typename _type_traits<T>::is_POD_type is_POD;
		return _uninitialized_copy_aux(first, last, result, is_POD());
	}
	template <typename ForwardIterator, typename InputIterator>
	inline ForwardIterator
		_uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, _true_type)
	{
		return copy(first, last, result);
	}

	template <typename ForwardIterator, typename InputIterator>
	inline ForwardIterator
		_uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, _false_type)
	{
		ForwardIterator curr = result;
		for (; first != last; ++first, ++curr)
			construc(&*curr, *first);
		return curr;
	}
	inline char* uninitialized_copy(const char* first, const char* last, char* result)
	{
		memmove(result, first, last - first);
		return result + (last - first);
	}
	inline wchar_t* uninitialized_copy(const wchar_t* first, const wchar_t* last, wchar_t* result)
	{
		memmove(result, first,sizeof(wchar_t)* last - first);
		return result + (last - first);
	}

	/*uninitialized_fill
	 *如果输出范围内的迭代器所指的内存未被初始化，
	 *在该范围内产生x的复制品*/

	template <typename ForwardIterator ,typename T>
	inline void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x)
	{
		_uninitialized_fill(first, last, x, value(first));
	}
	template <typename ForwardIterator, typename T,typename T1>
	inline void _uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x, T1*)
	{
		typedef typename _type_traits<T1>::is_POD_type is_POD;
		_uninitialized_fill_aux(first, last, x, is_POD());
	}

	template <typename ForwardIterator,typename T>
	inline void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, _true_type)
	{
		fill(first, last, x);
	}

	template <typename ForwardIterator, typename T>
	inline void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, _false_type)
	{
		ForwardIterator curr = first;
		for (; curr != last; ++curr)
			construct(&*curr, x);
	}
 }


#endif 