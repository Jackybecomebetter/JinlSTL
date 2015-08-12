#ifndef UTILITY_H_
#define UTILITY_H_

namespace Jinl{
	template <class T>
	void swap(T& a, T&b)
	{
		T temp = a;
		a = b;
		b = a;
	}

	template<class T1, class T2>
	struct pair{
		typedef T1 first_type;
		typedef T2 second_type;

		T1 first;
		T2 second;

		pair(){}
	};








}

#endif