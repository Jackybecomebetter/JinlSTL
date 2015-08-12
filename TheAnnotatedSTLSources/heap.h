#ifndef _HEAP_H_
#define _HEAP_H_

#include "Iterator.h"

namespace Jinl{

	/*��ʵ�ַ���ʹ�����Դ洢��ʽ����δ��Arrray��Vector�ĵ�0��Ԫ����Ϊ�ڱ���
	*���ڵ������Ϊ0�����ڵ�����Ϊi�������ӽڵ�����Ϊ2i+1���Һ��ӽڵ�������2i+2��
	*���к��ӽڵ�С�������ڵ�ʱ����ʾ����ڡ�*/
	template<class RandomAccessIterator_tag>
	inline void make_heap(RandomAccessIterator_tag first, RandomAccessIterator_tag last){
		_make_heap(first, last, value_type(first), distance_type(first));
	}

	template<class RandomAccessIterator_tag,class T,class Distance>
	void _make_heap(RandomAccessIterator_tag first, RandomAccessIterator_tag last, T*, Distance*){
		Distance len = last - first;
		if (len < 2) return;
		Distance parent = (len - 2) / 2;   
		//�ҳ���һ����Ҫ���ŵ�����ͷ����
		while (true){
			_adjust_heap(first, parent, len, T(*(first + parent)));
			if (parent == 0) return;
			parent--;
		}
	}

	//�����ѣ����ֶѵ������ԡ�
	template<class RandomAccessIterator_tag,class Distance,class T>
	void _adjust_heap(RandomAccessIterator_tag first, Distance holeIndex, Distance len, const T& value){
		Distance topIndex = holeIndex;
		Distance secondChild = 2 * holeIndex + 2;
		//����ڶ�����������С�ڳ��ȣ�����ڡ���ʱҲ��Ȼ���ڸýڵ������
		while (secondChild < len)
		{
			//�Ƚ������������ӵ�ֵ��ѡȡ����ֵ
			if (*(first + secondChild) < *(first + (secondChild - 1)))
				secondChild--;
			*(first + holeIndex) = *(first + secondChild);
			holeIndex = secondChild;
			secondChild = 2 * (secondChild + 1);
		}
		//û���Һ��ӽڵ㣬ֻ������
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

	/*pop_heap�����������������������ֵײ�������ͷβ��
	 *������������������ִ�н�����ܲ�����Ԥ�ڡ�*/
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