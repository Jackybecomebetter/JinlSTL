#ifndef _SET_H_
#define _SET_H_


namespace Jinl{

	template<class Key,class Compare = less<Key>>
	class set{
	public:
		typedef Key  key_type;
		typedef Key  value_type;

		typedef Compare key_compare;
		typedef Compare value_compare;

	private:
		template<class T>
		struct identity :public unary_function<T, T>{
			const T& operator()(const T& x) const {
				return x;
			}
		};


	};



}


#endif