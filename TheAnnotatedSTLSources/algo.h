#ifndef _ALGO_H_
#define _ALGO_H_

#include "Iterator.h"
namespace Jinl{

	template<class FowardIterator, class T, class Distance>
	ForwardIterator _lower_bound(ForwardIterator first, ForwardIterator last,
		const T& value, Distance*, ForwardIterator_tag){
		Distance len = 0;
		distance(first, last, len);
		Distance half;
		ForwardIterator middle;
		while (len > 0){
			half = len >> 1;
			middle = first;
			advance(middle, half);

			if (*middle < value){
				first = middle;
				++first;
				len = len - half - 1;
			}
			else len = half;
		}
		return first;
	}

	template<class ForwardIterator,class T>
	inline ForwardIterator lower_bound(ForwardIterator first,
		ForwardIterator last,
		const T& value){
		return _lower_bound(first, last, value, distance_type(first), iterator_category(first));
	}
	;
}

#endif