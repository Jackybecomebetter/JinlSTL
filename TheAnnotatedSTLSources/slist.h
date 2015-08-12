#ifndef _SLIST_H_
#define _SLIST_H_

#include <cstddef>
#include "Iterator.h"
#include "allocator.h"



namespace Jinl{
	struct slistNodeBase{
		slistNodeBase* next;
	};
	template <class T>
	struct slistNode :public slistNodeBase{
		T data;
	};

	inline slistNodeBase* slist_make_link(slistNodeBase* prevNode, slistNodeBase*newNode){
		newNode->next = prevNode->next;
		prevNode->next = newNode;
		return newNode;
	}

	inline size_t _slist_size(slistNodeBase* node){
		size_t result = 0;
		for (; node != 0; node = node->next)
			++result;
		return result;
	}

	struct slistIteratorBase{
		//typedef about the type_traits.
		typedef size_t size_type;
		typedef ptrdiff_t diffrence_type;
		typedef ForwardIterator_tag iterator_category;

		//define of the class-member
		slistNodeBase* node; //point to base node 

		//define of the class-function


		slistIteratorBase(slistNodeBase* x) :node(x){}

		void incr(){ node = node->next; }    //next position.

		bool operator==(const slistIteratorBase& x) const {
			return node == x.node;
		}
		bool operator!=(const slistIteratorBase& x) const {
			return !(node == x.node);
		}

	};

	//单向链表的迭代器结构
	template<class T, class Ref, class Ptr>
	struct slistIterator :public slistIteratorBase{
		typedef slistIterator<T, T&, T*>			 iterator;
		typedef slistIterator<T, const T&, const T*> const_iterator;
		typedef slistIterator<T, Ref, Ptr>           self;

		typedef T                value_type;
		typedef Ptr              pointer;
		typedef Ref              reference;
		typedef slistNode<T>     listNode;


		slistIterator(listNode* x) :slistIteratorBase(x){}
		slistIterator() :slistIteratorBase(0){}
		slistIterator(const iterator& x) :slistIteratorBase(x.node){}

		reference opeator*()const{ return ((listNode*)node)->data; }
		pointer   opeator->()const{ return &(operator*()); }

		self& operator++(){
			incr();
			return *this;
		}

		self operator++(int){
			self temp = *this;
			incr();
			return temp;
		}
	};

	template <class T, class alloc = Alloc>
	class slist{
	public:
		typedef T                value_type;
		typedef T*               pointer;
		typedef const T*         const_pointer;
		typedef T&               reference;
		typedef const T&         const_reference;
		typedef size_t           size_type;
		typedef ptrdiff_t        difference_type;

		typedef slistIterator<T, T&, T*>    iterator;
		typedef slistIterator<T, const T&, const T*> const_iterator;


	private:
		slistNodeBase head;

		typedef slistNode<T>       listNode;
		typedef slistNodeBase      listNodeBase;
		typedef slistIteratorBase  iteratorBase;
		typedef allocator<listNode> listNodeAllocator;

		static listNode* createNode(const value_type& val){
			listNode* node = listNodeAllocator::allocate();
			construct(&node->data, val);
			node->next = nullptr;
			return node;
		}

		static void destroyNode(listNode* node){
			destroy(&node->data);
			listNodeAllocator::deallocate(node);
		}

	public:
		slist(){ head.next = nullptr; }
		~slist(){ clear(); }

		iterator begin(){ return iterator((listNode*)head.next); }
		iterator end(){ return iterator(nullptr); }
		size_type size() const { return _slist_size(head.next); }
		bool empty() const{ return head.next == nullptr; }

		//两个slist相互交换
		void swap(slist& x){
			listNodeBase* tmp = head.next;
			head.next = x.head.next;
			x.head.next = tmp;
		}

		reference front(){
			return ((listNode*)head.next)->data;
		}

		void push_front(const value_type& val){
			slist_make_link(&head, createNode(x));
		}

		void pop_front(){
			listNode* node = (listNode*)head.next;
			head.next = node->next;
			destroyNode(node);
		}

	};









}

#endif