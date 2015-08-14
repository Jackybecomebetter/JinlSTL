#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

#include "Iterator.h"
#include "Vector.h"
#include "cstddef"
namespace Jinl{

	//定义bucket单个节点
	template<class Value>
	struct hashtable_node{
		hashtable_node* next;
		Value val;
	};

	template<class Value,class Key,class HashFcn,class ExtractKey,class EqualKey>
	struct hashtable_iterator{
		typedef hashtable<Value, Key, HashFcn, ExtractKey, EqualKey> hashtable;
		typedef hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey> iterator;
		typedef hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey> const_iterator;
		typedef hashtable_node<Value, Key, HashFcn, ExtractKey, EqualKey>  node;

		typedef ForwardIterator_tag iterator_category;
		typedef Value value_type;
		typedef ptrdiff_t difference_type;
		typedef size_t   size_type;
		typedef Value&   reference;
		typedef Value*   pointer;


		//struct member
		node* cur;       //迭代器目前所指节点
		hashtable* ht;   //保持对容器的连接关系

		hashtable_iterator(node* n, hashtable* tab) :cur(n), ht(tab){}
		hashtable_iterator(){}

		reference  operator*() const { return cur->val; }
		pointer    operator->() const{ return &(operator*()); }

		iterator& operator++();
		iterator operator++(int);
		bool operator==(const iterator& it) const {
			return cur == it.cur;
		}
		bool operator!=(const iterator& it) const {
			return cur != it.cur;
		}

	};

	template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey>
	hashtable_iterator<Value,Key,HashFcn,ExtractKey,EqualKey>& 
		hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey>::operator++(){
		const node* old = cur;
		cur = cur->next;
		if (!cur){
			size_type bucket = ht->bkt_num(old->val);
			while (!cur && ++bucket < ht->buckets.size())
				cur = ht->buckets[bucket];
		}
		return *this;
	}

	template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey>
	hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey>&
		hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey>::operator++(int){
		iterator tmp = *this;
		++*this;
		return tmp;
	}



}

#endif