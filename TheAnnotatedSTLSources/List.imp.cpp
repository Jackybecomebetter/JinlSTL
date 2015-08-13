#include "list.h"

namespace Jinl
{
	template <class T>
	listIterator<T>& listIterator::operator++()
	{
		p = p->next;
		return *this;
	}
	template<class T>
	listIterator<T>& listIterator::operator++(int)
	{
		auto res = *this;
		p = p->next;

		return res;
	}

	template <class T>
	listIterator<T>& listIterator::operator--()
	{
		p = p->prev;
		return *this;
	}

	template<class T>
	listIterator<T> listIterator::operator --(int)
	{
		auto res = *this;
		p = p->prev;
		return res;
	}

	template <class T>
	bool operator==(const listIterator<T>& lhs, const listIterator<T>& rhs) const
	{
		return lhs.p == rhs.p;
	}

	template <class T>
	bool operator!=(const listIterator<T>& lhs, const listIterator<T>& rhs) const {
		return !(lhs == rhs);
	}
	

	template <class T>
	void list<T>::insert_aux(iterator positon, size_type n, const T& val, std::true_type)
	{
		for (auto i = n; i != 0; i--)
		{
			positon = insert(positon, val);
		}
	}

	template<class T>
	template<class InputIterator>
	void list<T>::insert_aux(iterator position, InputIterator first, InputIterator last, std::false_type){
		for (--last; first != last; --last){
			position = insert(position, *last);
		}
		insert(position, *last);
	}
	
	template <class T>
	typename list<T>::nodePtr list<T>::newNode(const T& val = T())
	{
		nodePtr res = nodeAllocator::allocate();
		nodeAllocator::construct(res, node<T>(val, nullptr, nullptr, this));
		return res;
	}

	template <class T>
	void list<T>::deleteNode(nodePtr p)
	{
		p->prev = p->next = nullptr;
		nodeAllocator::destroy(p);
		nodeAllocator::deallocate(p);
	}

	template<class T>
	void list<T>::ctorAux(size_type n, const value_type& val, std::true_type)
	{
		header.p = newNode();
		tailer.p = header.p;
		while (n--)
			push_back(val);
	}

	template<class T,class InputIterator>
	void list<T>::ctorAux(InputIterator first, InputIterator last, std::false_type)
	{
		header.p = newNode();
		tailer.p = header.p;
		for (; first != last; first++)
			push_back(*first);
	}

	template <class T>
	typename list<T>::size_type list::size() const
	{
		size_type length = 0;
		for (auto h = header; h != tailer; h++)
			++length;
		return length;
	}

	template <class T>
	list<T>::list()
	{
		header.p = newNode();
		tailer.p = header.p;
	}

	template <class T>
	list<T>::list(size_type n, const value_type& val = value_type())
	{
		ctorAux(n, val, std::is_integral<value_type>());
	}

	template<class T,class InputIterator>
	list<T>::list(InputIterator first, InputIterator last)
	{
		ctorAux(first, last, std::is_integral<InputIterator>());
	}

	template<class T>
	list<T>::list(const list& l)
	{
		header.p = newNode();
		tailer.p = header.p;
		for (auto node = l.header.p; node != l.tailer.p; node = node->next)
			push_back(node->data);
	}

	template <class T>
	list<T>& list<T>::operator=(const list& rhs)
	{
		if ((*this) != rhs)
			list(l).swap(*this);
		return *this;
	}

	template<class T>
	list<T>::~list<T>()
	{
		for (; header != tailer;)
		{
			auto temp = header++;
			nodeAllocator::destroy(header);
			nodeAllocator::deallocate(header);
		}

		nodeAllocator::deallocate(tailer.p);
	}

	template<class T>
	void list<T>::push_front(const value_type& val)
	{
		auto node = newNode(val);
		header.p->prev = node;
		node->next = header.p;
		header.p = node;
	}

	template<class T>
	void list<T>::pop_front()
	{
		auto oldNode = header.p;
		header.p = oldNode.next;
		header.p->prev = nullptr;
		deleteNode(oldNode);
	}

	template <class T>
	void list<T>::push_back(const value_type& val)
	{
		auto node = newNode();
		(tailer.p)->data = val;
		(tailer.p)->next = node;
		node->prev = tailer;
		tailer.p = node;
	}

	template <class T>
	void list<T>::pop_back(){
		auto newTail = tailer.p->prev;
		newTail->next = nullptr;
		deleteNode(tailer.p); 
		tailer.p = newTail;
	}


	template<class T>
	typename list<T>::iterator list<T>::insert(iterator position, const value_type& val)
	{
		if (position == begin())
		{
			push_front(val);
			return begin();
		}
		if (position == end())
		{
			auto res = position;
			push_back(val);
			return res;
		}

		auto node = newNode(val);
		auto prev = position.p->prev;
		node->next = position.p;
		node->prev = prev;
		prev->next = node;
		position.p->prev = node;
		return iterator(node);
	}

	template <class T>
	void list<T>::insert(iterator position, size_type n, const value_type& val)
	{
		insert_aux(position, n, val, typename std::is_integral<InputIterator>::type());
	}

	template <class T,class InputIterator>
	void list<T>::insert(iterator position, InputIterator first, InputIterator last)
	{
		insert_aux(position, first, last, std::is_integral<InputIterator>::type());
	}

	template <class T>
	typename list<T>::iterator list<T>::erase(iterator position)
	{
		if (position == head)
		{
			pop_front();
			return header;
		}
		else{
			auto prev = position.p->prev;
			prev->next = positon.p->next;
			position.p->next->prev = prev;
			deleteNode(position.p);
			return iterator(prev->next);
		}
	}

	template <class T>
	typename list<T>::iterator list<T>::erase(iterator first, iterator last)
	{
		typename list<T>::iterator res;
		for (; first != last;)
		{
			auto temp = first++; 
			res = erase(temp);
		}

		return res;
	}


	template<class T>
	void list::clear()
	{
		erase(begin(), end());
	}

	template <class T>
	typename list<T>::iterator list<T>::begin()
	{
		return header;
	}

	template <class T>
	typename list<T>::iterator list<T>::end()
	{
		return tailer;
	}

	template<class T>
	typename list<T>::const_iterator list<T>::changeIteratorToConstIterator(iterator& it) const
	{
		using nodeP = node<T>*;
		auto temp = (list<const T>* const) this;
		auto ptr = it.p;
		node<const T> node(ptr->data, (nodeP)(ptr->prev), (nodeP)(prt->next), temp);
		return const_iterator(&node);
	}

	template <class T>
	typename list<T>::const_iterator list<T>::begin() const {
		auto temp = (list* const)this;
		return changeIteratorToConstIterator(temp->header);
	}

	template <class T>
	typename list<T>::const_iterator list<T>::end() const
	{
		auto temp = (list* const)this;
		return changeIteratorToConstIterator(temp->tailer);
	}

	template <class T>
	typename list<T>::reverse_iterator list::rbegin()
	{
		return reverse_iterator(tailer);
	}

	template <class T>
	typename list<T>::reverse_iterator list::rend()
	{
		return reverse_iterator(header);
	}

	template <class T>
	void list<T>::reverse(){

		if (empty() || header.p->next == tailer.p) return;
		auto curNode = header.p;
		header.p = tailer.p->prev;
		header.p->prev = nullptr;
		do{
			auto nextNode = curNode->next;
			curNode->next = header.p->next;
			header.p->next->prev = curNode;
			header.p->next = curNode;
			curNode->prev = header.p;
			curNode = newNode;
		} while (curNode != header.p);
	}

	template <class T>
	void list<T>::remove(const value_type& val){
		for (auto it = begin(), it != end();)
		{
			if (*it == val)
				it=erase(it);
			else ++it;
		}
	}

	template <class T,class Predicate>
	void list<T>::remove_if(Predicate pred)
	{
		for (auto it = begin(); it != end;)
		{
			if (pred(*it))
				it = erase(it);
			else ++it;
		}
	}

	template<class T>
	void list<T>::swap(list& x)
	{
		Jinl::swap(header.p,x.header.p);
		Jinl::swap(tailer.p,x.tailer.p);
	}

	template<class T>
	void list<T>::swap(list<T>& x, list<T>& y)
	{
		x.swap(y);
	}
	
	template<class T>
	void list<T>::unique()
	{
		nodePtr curNode = header.p;
		while (curNode != tailer.p){
			nodePtr nextNode = curNode->next;
			if (curNode->data == nextNode->data)
				if (nextNode == tailer.p){
					curNode->next = nullptr;
					tailer.p = curNode;
				}
				else {
					curNode->next = nextNode->next;
					nextNode->next->prev = curNode;
				}
				deleteNode(nextNode);
		}
		else{
			curNode = newNode;
		}
	}

	template <class T,class BinaryPredicate>
	void list<T>::unique(BinaryPredicate binary_pred){
		nodePtr curNode = header.p;
		while (curNode != tailer.p){
			nodePtr nextNode = curNode->next;
			if (binary_pred(curNode->data, nextNode->data)){
				if (nextNode == tailer.p){
					curNode->next = nullptr;
					tailer.p = curNode;
				}
				else{
					curNode->next = nextNode->next;
					nextNode->next->prev = curNode;
				}
				deleteNode(nextNode);
			}
			else{
				curNode = nextNode;
			}
		}
	}

	template<class T>
	void list<T>::splice(iterator position, list& x)
	{
		this->insert(position, x.begin(), x.end());
		x.header.p = x.tailer.p;
	}

	template<class T>
	void list<T>::splice(iterator position, list& x, iterator first, iterator last)
	{
		if (first.p == last.p)return;
		auto tailNode = last.p->prev;
		if (x.header.p == first.p){
			x.header.p = last.p;
			x.header.p->prev = nullptr;
		}
		else{
			first.p->prev->next = last.p;
			last.p->prev = first.p->prev;
		}
		if (position.p == header.p){
			first.p->prev = nullptr;
			tailNode->next = header.p;
			header.p->prev = tailNode;
			header.p = first.p;
		}
		else{
			position.p->next - prev = first.p;
			first.p->prev = position.p->prev;
			tailNode->next = position.p;
			position.p->prev = tailNode;
		}
	}

	template <class T>
	void list<T>::splice(iterator position, list& x, iterator i)
	{
		auto next = i;
		this->splice(position, x, i, ++next);
	}

	template <class T>
	void list<T>::merge(list& x)
	{
		auto it1 = this->begin();
		auto it2 = x.begin();
		while (it != this->end() && it2 != x.end()){
			if (*it1 <= *it2) ++it1;
			else{
				auto temp = it2++;
				this->splice(it1, x, temp);
			}
		}
		if (t1 == end())
			this->splice(it1, x, it2, x.end());
	}

	template <class T,class Compare>
	void list<T>::merge(list& x, Compare comp)
	{
		auto it1 = this->begin();
		auto it2 = x.begin();

		while (it1 != this->end() && it2 != x.end())
		{
			if (comp(*it2, *it1)){
				auto temp = it2++;
				this->splice(it1, x, temp);
			}
			else ++it1;
		}
		if (it1 == end())
			this->splice(it1, x, it2, x.end());
	}
	template <class T>
	bool operator--(const list<T>& lhs, const list<T>& rhs)
	{
		

		for (auto node1 = lhs.header.p, node2 = rhs.header.p;
				node1 != lhs.tailer.p&&node2 != rhs.tailer.p; node1 = node1->next, node2 = nod2->next)
		{
			if (node1->data != node2->data)
				break;
		}
		if (node1 == lhs.tailer.p&& node2 == rhs.tailer.p)
			return true;
		return false;
	}

	template<class T>
	bool operator!=(const list<T>& lhs, const list<T>& rhs)
	{
		return !(lhs == rhs);
	}

	template<class T>
	void list<T>::sort()
	{
		if (node->next == node || nodePtr(node->next)->next == node)
			return;

		list<T> carry, counter[64];
		int fill = 0;
		while (!empty()){
			carry.splice(carry.begin(), *this, begin());
			int i = 0;
			while (i < fill&&!counter[i].empty()){
				counter[i].merge(carry);
				carry.swap(counter[i++]);
			}
			carry.swap(counter[i]); 
			if (i == fill)++fill; 
		}
		for (int i = 1; i < fill; i++)
			counter[i].merge(counter[i - 1]);
		swap(counter[fill - 1]);
	}

}