#include "stack.h"

namespace Jinl{

	template <class T, class Sequence = deque<T>>
	bool stack::empty() const {
		return c.empty();
	}

	template <class T, class Sequence = deque<T>>
	size_type stack::size() const{
		return c.size();
	}
	template <class T, class Sequence = deque<T>>
	reference stack::top(){
		return c.back();
	}
	template <class T, class Sequence = deque<T>>
	const_reference stack::top() const {
		return c.back();
	}

	template <class T, class Sequence = deque<T>>
	void push(const value_type& val){
		c.push_back(val);
	}
	template <class T, class Sequence = deque<T>>
	void stack::pop(){
		c.pop_back();
	}

	template <class T, class Sequence = deque<T>>
	bool operator==(const stack<T, Sequence>& lhs, const stack<T, Sequence>& rhs){
		return lhs.c == rhs.c;
	}
	template <class T, class Sequence = deque<T>>
	bool operator!=(const stack<T, Sequence>& lhs, const stack<T, Sequence>& rhs){
		return !(lhs.c == rhs.c);
	}

	template <class T, class Sequence = deque<T>>
	bool operator<(const stack<T, Sequence>& lhs, const stack<T, Sequence>& rhs){
		return lhs.c < rhs.c;
	}

}