#ifndef _ITERATOR_H_
#define _ITERATOR_H_

namespace Jinl
{
	struct InputIterator_tag{};
	struct OutputIterator_tag{};
	struct ForwardIterator_tag : public InputIterator_tag{};
	struct BidirectionIterator_tag : public ForwardIterator_tag{};
	struct RandomAccessIterator_tag : public BidirectionIterator_tag{};

	template <class Category,class T,class Distance = ptrdiff_t,class Pointer = T*,class Reference = T&>
	struct Iterator{
		typedef Category     iterator_category;
		typedef T            value_type;
		typedef Distance     difference_type;
		typedef Pointer      pointer;
		typedef Reference    reference;
	};

	//traits
	template <class Iterator>
	struct iterator_traits
	{
		typedef typename Iterator::iterator_category  iterator_category;
		typedef typename Iterator::value_type         value_type;
		typedef typename Iterator::difference_type    difference_type;
		typedef typename Iterator::pointer            pointer;
		typedef typename Iterator::reference          reference;
	};

	//针对原生指针的偏特化版本
	template <class T>
	struct iterator_traits<T*>
	{
		typedef RandomAccessIterator_tag              iterator_category;
		typedef T                                     value_type;
		typedef T*                                    pointer;
		typedef T&                                    reference;
		typedef ptrdiff_t                             diffrence_type;
	};
	//针对原生常指针的偏特化版本
	template <class T>
	struct iterator_traits<const T*>
	{
		typedef RandomAccessIterator_tag              iterator_category;
		typedef T                                     value_type;
		typedef T*                                    pointer;
		typedef T&                                    reference;
		typedef ptrdiff_t                             diffrence_type;
	};

	template <class Iterator>
	inline typename iterator_traits<Iterator>::iterator_category
		iterator_category(const Iterator&)
	{
		typedef typename iterator_traits<Iterator>::iterator_category category;
		return category();
	}

	template <class Iterator>
	inline typename iterator_traits<Iterator>::difference_type*
		distance_type(const Iterator&)
	{
		return static_cast<typename iterator_traits<Iterator>::difference_type>(0);
	}
	template <class Iterator>
	inline typename iterator_traits<Iterator>::value_type*
		value_type(const Iterator&)
	{
		return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
	}

	template <class InputIterator>
	inline iterator_traits<InputIterator>::difference_type
		__distance(InputIterator first, InputIterator last, InputIterator_tag)
	{
		iterator_traits<InputIterator>::difference_type n = 0;
		while (first != last)
		{
			++first;
			++n;
		}
		return n;
	}

	template <class RandomAccessIterator>
	inline iterator_traits<RandomAccessIterator>::difference_type
		__distance(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator_tag)
	{
		return last - first;
	}
	template<class InputIterator>
	inline iterator_traits<InputIterator>::difference_type
		distance(InputIterator first, InputIterator last)
	{
		typedef typename
			iterator_traits<InputIterator>::iterator_category category;
		return __distance(first, last, category());
	}

	template <class InputIterator,class Distance>
	inline void __advance(InputIterator& i, Distance n, InputIterator_tag)
	{
		while (n--) i++;
	}
	template <class ForwardIterator,class Distance>
	inline void __advance(ForwardIterator& i, Distance n, ForwardIterator_tag)
	{
		__advance(i, n, InputIterator_tag());
	}

	template <class BidirectionIterator,class Distance>
	inline void __advance(BidirectionIterator& i, Distance n, BidirectionIterator_tag)
	{
		if (n >= 0){
			while (n--)i++;
		}
		else{
			while (n++) i--;
		}
	}

	template <class RandomAccessIterator,class Distance>
	inline void __advance(RandomAccessIterator& i, Distance n,RandomAccessIterator_tag)
	{
		i += n;
	}

	template <class InputIterator,class Distance>
	inline void  advance(InputIterator& i, Distance n)
	{
		__advance(i, n, iterator_traits<InputIterator>::iterator_category(i));
	}
}


#endif