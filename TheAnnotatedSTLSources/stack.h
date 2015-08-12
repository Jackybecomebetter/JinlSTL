#ifndef _STACK_H_
#define _STACK_H_

#include <deque.h>

namespace Jinl{
	template <class T, class Sequence = deque<T>>
	class stack{
		friend bool operator== (const stack& lhs, const stack& rhs);
		friend bool operator!=(const stack& lhs, const stack& rhs);
		friend bool operator<(const stack& lhs, const stack& rhs);

	public:

		typedef typename Sequence::value_type value_type;
		typedef typename Sequence::size_type size_type;
		typedef typename Sequence::reference reference;
		typedef typename Sequence::const_reference const_reference;

	protected:
		Sequence c;
	//采用适配器技术，所有操作都在低层的容器上完成。
	public:
		bool empty() const;
		size_type size() const;
		reference top();
		const_reference top() const;
		void push(const value_type& val);
		void pop();

	};

}


#endif