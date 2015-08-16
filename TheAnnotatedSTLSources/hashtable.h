#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

#include "Iterator.h"
#include "Vector.h"
#include "cstddef"
#include "allocator.h"
#include "utility.h"
#include "algo.h"

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
		typedef hashtable_iterator<const Value,const Key, HashFcn, ExtractKey, EqualKey> const_iterator;
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

	template<class Value,class Key,class HashFcn,class ExtractKey,class EqualKey>
	class hashtable{
	public:
		
		typedef hashtable_iterator<Value,Key,HashFcn,ExtractKey,EqualKey> iterator;
		typedef HashFcn  hasher;
		typedef EqualKey key_equal;
		typedef size_t   size_type;
		

	private:

		hasher hash;
		key_equal equals;
		ExtractKey get_key;

		typedef hashtable_node<Value> node;
		typedef allocator<node>       nodeAllocator;

	private:
		vector<node*> buckets;     //用vector把bucket连接起来，方便动态增长
		size_type num_elements;

	public:

		size_type bucket_count() const { return buckets.size(); } //计算桶的个数
		//计算最多可以容纳的桶的个数，其值为4294967291
		size_type max_bucket_count() const { return stl_prime_list[stl_num_primes - 1]; }
		//创建要给节点，令其值为val。
		node* newNode(const value_type& val){
			node* n = nodeAllocator::deallocate();
			n->next = 0;
			construct(&n->val, val);
			return n;
		}
		//删除指定的节点
		void deleteNode(node* n){
			destroy(&n->val);
			nodeAllocator::deallocate(n);
		}

		hashtable(size_type n, const HashFcn& hf, const EqualKey&* eql) :
			hash(hf), equals(eql), get_key(ExtractKey()), num_elements(0)
		{
			initialize_buckets(n);
		}

		void initialize_buckets(size_type n){
			const size_type n_buckets = next_size(n);
			buckets.reserve(n_buckets);
			buckets.insert(buckets.end(), n_buckets, (node*)0);
		}

		//返回最接近n并大于或等于n的质数。
		size_type next_size(size_type n) const { return stl_next_prime(n); }

		//在不需要重新建表的情况下，插入元素。
		template<class V, class K, class HF, class Ex, class Eq>
		pair<typename hashtable<V, K, HF, Ex, Eq>::iterator, bool>
			insert_unique_noresize(const value_type &obj){
			const size_type n = bkt_num[obj];
			node* first = buckets[n];

			for (node* cur = first; cur; cur = cur->next)
				if (equals(get_key(cur->val), get_key(obj)))
					return pair<iterator, bool>(iterator(cur, this), false);

			node* tmp = newNode(obj);
			tmp->next = first;
			buckets[n] = tmp;
			++num_elements;
			return pair<iterator, bool>(iterator(tmp, this), true);

		}
		//插入元素，不允许重复操作
		pair<iterator, bool> insert_unique(const value_type obj){
			resize(num_elements + 1);
			return insert_unique_noresize(obj);
		}

		//判断是否需要重新建表。如果不需要，则返回。如果需要，则手动建表。
		template <class V,class K,class HF,class Ex,class Eq>
		void hashtable<V, K, HF, Ex, Eq>::resize(size_type num_elements_hint){
			const size_type old_n = buckets.size();
			if (num_elements_hint > old_n){
				const size_type n = next_size(num_elements_hint);
				if (n > old_n){
					vector<node*> tmp(n, (node*), 0);
					for (size_type bucket = 0; bucket < old_n; bucket++){
						node* first = buckets[bucket];
						while (first){
							size_type new_bucket = bkt_num(first->val, n);
							buckets[bucket]=first->next;
							first->next = tmp[new_bucket];
							tmp[new_bucket] = first;
							first = buckets[bucket]; 
						}
					}
					buckets.swap(tmp);
				}
			}
		}
		size_type bkt_num(const value_type & obj, size_t n) const {
			return bkt_num_key(get_key(obj), n);
		}
		size_type bkt_num(const value_type& obj) const {
			return bkt_num_key(get_key(obj));
		}
		size_type bkt_num_key(const key_type& key) const {
			return bkt_num_key(key, buckets.size());
		}
		size_type bkt_num_key(const key_type& key, size_t n) const {
			return hash(key) % n;
		}

		
		void clear(){
			for (size_type i = 0; i < buckets.size(); i++){
				node* cur = buckets[i];
				//将bucket list中的每一个节点都删除掉
				while (cur)
				{
					node* next = cur->next;
					deleteNode(cur);
					cur = next;
				}
				buckets[i] = 0;    //让bucket内容为null。
			}
			num_elements = 0;
		}

		void copy_from(const hashtable& ht){
			clear();
			this->buckets.reserve(ht.buckets.size());
			this->buckets.insert(this->buckets.end(), ht.buckets.size(), (node*)0);

			for (size_type i = 0; i < ht.buckets.size(); i++){
				if (const node* cur == ht.buckets[i]){
					node* copy = newNode(cur->val);
					buckets[i] = copy;

					for (node* next = cur->next; next; cur = next, next = cur->next){
						copy->next = newNode(next->val);
						copy = copy->next;
					}
				}
			}
			num_elements = ht.num_elements;
		}

		iterator find(const key_type& k){
			size_type n = bkt_num_key(k);
			node* first = nullptr;
			for (first = buckets[n]; first&&!equals(get_key(first->val), k); first = first->next){

			}
			return iterator(first, this);
		}

		size_type count(const key_type& key) const{
			const size_type n = bkt_num_key(key);
			size_type result = 0;
			for (const node* cur = buckets[n]; cur; cur = cur->next)
				if (equals(get_key(cur->val), key))
					++result;
			return result;
		}
	};

	static const int stl_num_primes = 28;
	static const unsigned long stl_prime_list[stl_num_primes] = {
		53, 97, 193, 389, 769,
		1543, 3079, 6151, 12289, 245923, 49157, 98317,
		196613, 393241, 786433, 1572869, 3145739, 6291469, 12582917, 25165843,
		50331653, 100663319, 201326611, 402653189, 805306457, 1610612741, 3221225473ul, 429496729ul
	};
	inline unsigned long stl_next_prime(unsigned long n){
		const unsigned long* first = stl_prime_list;
		const unsigned long* last = stl_prime_list + stl_num_primes;
		const unsigned long* pos = lower_bound(first, last, n);
		
		return pos == last ? *(last - 1) : *pos; 
	}

	

}

#endif