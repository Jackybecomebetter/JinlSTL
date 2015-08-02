#ifndef _CONSTRUCT_H_
#define _CONSTRUCT_H_

#include <new.h>
#include "TypeTraits.h"


namespace Jinl
{
	template<class T1,class T2>
	inline void construct(T1* ptr, const T2& value)
	{
		new(ptr) T1(value);
	}

	template <class T>
	inline void destroy(T* pointer)
	{
		pointer->~T();
	}

	template<class ForwardIterator>
	inline void destroy_(ForwardIterator first, ForwardIterator last, _true_type)
	{
	
	}

	template<class ForwardIterator>
	inline void destroy_(ForwardIterator first, ForwardIterator last, _false_type){
		for (; first != last; ++first){
			destroy(&*first);
		}
	}

	template<class ForwardIterator>
	inline void destroy(ForwardIterator first, ForwardIterator last){
		typedef typename _type_traits<T>::is_POD_type is_POD_type;
		destroy_(first, last, is_POD_type);
	}
	inline void destroy(char*, char*){}
	inline void destroy(wchar_t*, wchar_t*){}


}

#endif