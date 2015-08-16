#ifndef _NUMERIC_H_
#define _NUMERIC_H_

namespace Jinl{

	template<class Iterator,class T>
	Iterator find(Iterator first, Iterator last, const T& value){
		while (first != last&&*first != value)
			first++;
		return first;
	}

	template<class InputIterator,class T>
	T accumulate(InputIterator first, InputIterator last, T init){
		for (; first != last; first++)
			init = init + *first;
		return init;
	}

	//[first,last)�����Ԫ��ʩ��binary_op�����������ӵ���ʼֵinit֮�ϡ�
	template<class InputIterator,class T,class BinaryOperation>
	T accumulate(InputIterator first, InputIterator last, T init, BinaryOperation binary_op)
	{
		for (; first != last; first++)
			init = init + binary_op(init, *first);
		return init;
	}

	/*adjacent_difference,��������Ԫ�صĲ�ֵ��*/
	template<class InputIterator,class OutputIterator>
	OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator result){
		if (first == last) return result;
		*result = *first;
		return _adjacent_difference(first, last, result, value_type(first));
	}
	template<class InputIterator,class OutputIterator,class T>
	OutputIterator _adjacent_difference(InputIterator first, InputIterator last, OutputIterator result, T*){
		T value = *first;
		while (++first!=last)
		{
			T tmp = *first;
			*++result = tmp - value;
			value = tmp;
		}
		return ++result;
	}

	template<class InputIterator,class OutputIterator,class BinaryOperation>
	OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator result,
		BinaryOperation binary_op)
	{
		if (first == last) return result;
		*result = *first;
		return _adjacent_difference(first, last, result, value_type(first), binary_op);
	}

	template<class InputIterator,class OutputIterator,class T,class BinaryOperation>
	OutputIterator _adjacent_difference(InputIterator first, InputIterator last, OutputIterator result, T*, BinaryOperation binary_op){
		T value = *first;
		while (++first != last)
		{
			T tmp = *first;
			*++result = binary_op(tmp, value);
			value = tmp;
		}
		return ++result;
	}

	/*inner product �������ڻ�*/
	template<class InputIterator1,class InputIterator2,class T>
	T inner_product(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2,
		T init)
	{
		for (; first1 != last1; ++first1, ++first2)
			init = init + (*first1 * *first1);
		return init;
	}

	template<class InputIterator1,class InputIterator2,class T,class BinaryOperation1,class BinaryOperation2>
	T inner_product(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2, T init, 
		BinaryOperation1 binary_op1, BinaryOperation2 binary_op2){

		for (; first1 != last1; first1++��first2++)
			init = binary_op1(init, binary_op2(*first1, *first2));
		return init;
	}

	/*partial_sum���ֲ����,����ֵ��ǰ����Ԫ�صĺ�*/
	template<class InputIterator,class OutputIterator>
	OutputIterator partial_sum(InputIterator first, InputIterator last, OutputIterator result){
		if (first == last)return result;
		*result = *first;         //����һ����ֵ��
		return _partial_sum(first, last, result, value_type(first));
	}

	template<class InputIterator, class OutputIterator,class T>
	OutputIterator _partial_sum(InputIterator first, InputIterator last, InputIterator result, T*){
		T value = *first;
		while (++first!=last)
		{
			value = value + first;
			*++result = value;
		}
		return ++result;
	}

	template<class InputIterator,class OutputIterator,class BinaryOperation>
	OutputIterator partial_sum(InputIterator first, InputIterator last,OutputIterator result,
		BinaryOperation binary_op){
		if (first == last) return result;
		*result = *first;
		return _partial_sum(first, last, result, value_type(first), binary_op);
	}
	template<class InputIterator,class OutputIterator,class BinaryOperation,class T>
	OutputIterator _partial_sum(InputIterator first, InputIterator last,
		OutputIterator result, T*, BinaryOperation binary_op){

		T value = *first;
		while (++first!=last)
		{
			value = binary_op(value, *first);
			*++result = value;
		}
		return ++result;
	}

	/*power�����ڼ���ĳ����n�ݴη���ָ�Լ����Լ�����ĳ�ּ����n�Ρ�*/
	template<class T,class Integer>
	inline T power(T x, Integer n){
		return power(x, n, multiplise<T>());
	}

	template<class T,class Integer,class MonoidOperation>
	T power(T x, Integer n, MonoidOperation op){
		if (n == 0)
			return identity_elements(op);    //����֤ͬԪ��
		else{
			while ((n & 1) == 0){
				n >>= 1;
				x = op(x, x);
			}
			T result = x;
			n >>= 1;
			while (n != 0){
				x = op(x, x);
				if ((n & 1) == 0)
					result = op(result, x);
				n >> 1;
			}
			return result;
		}
	}

	template<class ForwardIterator,class T>
	void iota(ForwardIterator first, ForwardIterator last, T value){
		while (first != last)*first++ = value++;
	}




}

#endif