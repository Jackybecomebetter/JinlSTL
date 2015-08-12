#ifndef _HEAP_H_
#define _HEAP_H_

#include "Iterator.h"

namespace Jinl{

	/*该实现方法使用线性存储方式，并未将Arrray或Vector的第0个元素设为哨兵。
	*根节点的索引为0，父节点坐标为i，则左孩子节点坐标为2i+1，右孩子节点坐标是2i+2。
	*当有孩子节点小于索引节点时，表示其存在。*/
	template<class RandomAccessIterator_tag>
	inline void make_heap(RandomAccessIterator_tag first, RandomAccessIterator_tag last){
		_make_heap(first, last, value_type(first), distance_type(first));
	}

	template<class RandomAccessIterator_tag,class T,class Distance>
	void _make_heap(RandomAccessIterator_tag first, RandomAccessIterator_tag last, T*, Distance*){
		Distance len = last - first;
		if (len < 2) return;
		Distance parent = (len - 2) / 2;   
		//找出第一个需要重排的子树头部。
		while (true){
			_adjust_heap(first, parent, len, T(*(first + parent)));
			if (parent == 0) return;
			parent--;
		}
	}

	//调整堆，保持堆的有序性。
	template<class RandomAccessIterator_tag,class Distance,class T>
	void _adjust_heap(RandomAccessIterator_tag first, Distance holeIndex, Distance len, const T& value){
		Distance topIndex = holeIndex;
		Distance secondChild = 2 * holeIndex + 2;
		//如果第二个孩子索引小于长度，则存在。此时也必然存在该节点的左孩子
		while (secondChild < len)
		{
			//比较左右两个孩子的值，选取最大的值
			if (*(first + secondChild) < *(first + (secondChild - 1)))
				secondChild--;
			*(first + holeIndex) = *(first + secondChild);
			holeIndex = secondChild;
			secondChild = 2 * (secondChild + 1);
		}
		//没有右孩子节点，只有左孩子
		if (secondChild == len){
			*(first + holeIndex) = *(first + (secondChild - 1));
			holeIndex = secondChild - 1;
		}
		_push_heap(first, holeIndex, topIndex, value);
	}
	template<class RandomAccessIterator_tag>
	inline void push_heap(RandomAccessIterator_tag first, RandomAccessIterator_tag last){
		_push_heap_aux(first, last, distance_type(first), value_type(first));
	}

	template<class RandomAccessIterator_tag,class Distance,class T>
	inline void _push_heap_aux(RandomAccessIterator_tag first, RandomAccessIterator_tag last,
		Distance*, T*){
		_push_heap(first, Distance((last - first) - 1), Distance(0), T(*(last - 1)));
	}

	template <class RandomAccessIterator_tag,class Distance,class T>
	void _push_heap(RandomAccessIterator_tag first, Distance holeIndex, Distance topIndex, T value){
		Distance parent = (holeIndex - 1) / 2;
		while (holeIndex > topIndex&&*(first + parent) < value){
			*(first + holeIndex) = *(first + parent);
			holeIndex = parent;
			parent = (holeIndex - 1) / 2;
		}
		*(first + holeIndex) = value;
	}

	/*pop_heap接受两个迭代器，用来表现底部容器的头尾，
	 *如果不符合这个条件，执行结果可能不符合预期。*/
	template<class RandomAccessIterator_tag>
	inline void pop_heap(RandomAccessIterator_tag first, RandomAccessIterator_tag last){
		_pop_heap_aux(first, last, value_type(first));
	}

	template<class RandomAccessIterator_tag,class T>
	inline void _pop_heap_aux(RandomAccessIterator_tag first, RandomAccessIterator_tag last, T*){
		
		_pop_heap(first, last - 1, last - 1, T(*(last - 1)), distance_type(first));
	}

	template<class RandomAccessIterator_tag,class T,class Distance>
	inline void _pop_heap(RandomAccessIterator_tag first,
		RandomAccessIterator_tag last,
		RandomAccessIterator_tag result,
		const T& value, Distance*)
	{
		*result = *first;
		_adjust_heap(first,Distance(0),Distance(last-first),value);
	}

	template<class RandomAccessIterator_tag>
	void sort_heap(RandomAccessIterator_tag first, RandomAccessIterator_tag last){
		while (last - first > 1)
			pop_heap(first, last--);
	}

}


#endif