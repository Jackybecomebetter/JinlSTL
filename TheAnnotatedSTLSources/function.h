#ifndef _FUNCTION_H_
#define _FUNCTION_H_

namespace Jinl{

	template<class T>
	struct less{
		typedef T first_type;
		typedef T second_type;
		typedef bool result_type;

		result_type operator()(const first_type& x, const second_type& y){
			return x < y;
		}

	};
	template<class T>
	struct equal_to{
		typedef T first_type;
		typedef T second_type;
		typedef bool result_type;

		result_type operator()(const first_type& x, const second_type& y){
			return x == y;
		}

	};
	



}


#endif