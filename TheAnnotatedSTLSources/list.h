#ifndef _LIST_H_
#define _LIST_H_

#include "Iterator.h"
#include "allocator.h"
namespace Jinl{

	//声明列表，在node中被使用
	template <class T>
	class list;

	//定义双向列表节点
	template <class T>
	struct node {

		node* prev;
		node* next;
		T data;
		list<T>* container;

		node(const T& d, node* p, node* n, list<T>* l) :data(d), prev(p), next(n), container(l)
		{

		}

		//拷贝赋值方法
		node(const node& n) :data(n.data), prev(n.prev), next(n.next), container(n.container)
		{

		}
		node& operator=(const node& rhs){
			this->data = rhs.data;
			this->prev=rhs.prev;
			this->next=rhs.next;
			this->container = rhs.container;

			return *this;
		}

		bool operator==(const node& n)
		{
			return data == n.data && prev == n.prev && next == n.next && container == n.container;
		}
		bool operator!=(const node& n)
		{
			return !(*this == n);
		}

	};

	//定义双向列表的迭代器，支持双向操作
	template<class T>
	struct listIterator :public Iterator<BidirectionIterator_tag, T>
	{
		template <class T> 
		friend class list;      //list 可以访问listiterator
		
		typedef	node<T>* nodePtr;       //在节点级进行访问，指示前后元素
		nodePtr p;
	public:
		explicit listIterator(nodePtr ptr = nullptr) :p(ptr){}

		listIterator& operator++();
		listIterator operator++(int);
		listIterator& operator --();
		listIterator operator --(int);
		T& operator *(){ return p->data; }
		T* operator ->(){ return &(operator*()); }

		

		template<class T>
		friend bool operator ==(const listIterator<T>& lhs, const listIterator<T>& rhs);
		template<class T>
		friend bool operator !=(const listIterator<T>& lhs, const listIterator<T>& rhs);


	};
	template <class T>
	class list {
		template<class T>
		friend struct listIterator;
	private:
		typedef allocator<node<T>> nodeAllocator;
		typedef node<T>* nodePtr;

	public:
		typedef T               value_type;
		typedef listIterator<T> iterator;
		typedef T*              pointer;
		typedef T&              reference;
		typedef size_t          size_type;
		typedef listIterator<const T> const_iterator;
		typedef reverse_iterator_t<T>  reverse_iterator;

	private:
		iterator header;
		iterator tailer;
	public:
		list();
		explicit list(size_type n, const value_type& val = value_type());
		template <class InputIterator>
		list(InputIterator first, InputIterator last);
		list(const list& l);
		list& operator = (const list& l);
		~list();

		bool empty()const{ return header == tailer; }
		size_type size()const;
		reference front(){ return (header.p->data); }
		reference back(){ return (tailer.p->prev->data); }

		void push_front(const value_type& val);
		void pop_front();
		void push_back(const value_type& val);
		void pop_back();

		iterator begin();
		iterator end();
		const_iterator begin()const;
		const_iterator end()const;
		reverse_iterator rbegin();
		reverse_iterator rend();

		iterator insert(iterator position, const value_type& val);
		void insert(iterator position, size_type n, const value_type& val);
		template <class InputIterator>
		void insert(iterator position, InputIterator first, InputIterator last);
		iterator erase(iterator position);
		iterator erase(iterator first, iterator last);
		void swap(list& x);
		void clear();
		void splice(iterator position, list& x);
		void splice(iterator position, list& x, iterator i);
		void splice(iterator position, list& x, iterator first, iterator last);
		void remove(const value_type& val);
		template <class Predicate>
		void remove_if(Predicate pred);
		void unique();
		template <class BinaryPredicate>
		void unique(BinaryPredicate binary_pred);
		void merge(list& x);
		template <class Compare>
		void merge(list& x, Compare comp);
		void sort();
		template <class Compare>
		void sort(Compare comp);
		void reverse();
	private:
		void ctorAux(size_type n, const value_type& val, std::true_type);
		template <class InputIterator>
		void ctorAux(InputIterator first, InputIterator last, std::false_type);
		nodePtr newNode(const T& val = T());
		void deleteNode(nodePtr p);
		void insert_aux(iterator position, size_type n, const T& val, std::true_type);
		template<class InputIterator>
		void insert_aux(iterator position, InputIterator first, InputIterator last, std::false_type);
		const_iterator changeIteratorToConstIterator(iterator& it)const;
	public:
		template<class T>
		friend void swap(list<T>& x, list<T>& y);
		template <class T>
		friend bool operator== (const list<T>& lhs, const list<T>& rhs);
		template <class T>
		friend bool operator!= (const list<T>& lhs, const list<T>& rhs);

	};

}

#endif