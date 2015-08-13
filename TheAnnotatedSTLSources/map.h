#ifndef _MAP_H_
#define _MAP_H_

#include "RBTree.h"
#include "Iterator.h"
#include "utility.h"
#include "ReverseIterator.h"

namespace Jinl{

	template<class Key,class T,class Compare = less<Key>>
	class map{
	public:
		typedef Key key_type;
		typedef T   data_type;
		typedef T   mapped_type;
		typedef pair<const Key, T> value_type;
		typedef Compare key_compare;           //键值比较函数


		class value_compare :
			public binary_function<value_type, value_type, bool>{
			friend class map<Key, T, Compare>;
		protected:
			Compare comp;
			value_compare(Compare c) :comp(c){}
		public:
			bool operator()(const value_type& x, const value_type& y)const{
				return comp(x.first, y.first);
			}
		};


	private:

		typedef rbTree<key_type, value_type, select1st<value_type>, key_compare> rep_type;
		rep_type t;

	public:
		typedef typename rep_type::pointer  pointer;
		typedef typename rep_type::const_pointer const_pointer;
		typedef typename rep_type::iterator iterator;
		typedef typename rep_type::const_iterator const_iterator;
		typedef typename rep_type::reference reference;
		typedef typename rep_type::const_reference const_reference;
		typedef typename rep_type::reverse_iterator_t reverse_iterator;
		typedef typename rep_type::const reverse_iterator_t const_reverse_iterator;
		typedef typename rep_type::size_type size_type;
		typedef typename rep_type::difference_type difference_type;

		map() :t(Compare()){}
		explicit map(const Compare& comp) :t(comp){}

		template<class InputIterator>
		map(InputIterator first,InputIterator last):
			t(Compare()){
			t.insert_unique(first, last);
		}

		template<class InputIterator>
		map(InputIterator first, InputIterator last, const Compare& comp) :
			t(comp){
			t.insert_unique(first, last);
		}

		map(const map<Key, T, Compare>& x) :t(x.t){}

		map<Key, T, Compare>& operator=(const map<Key, T, Compare>& x){
			t = x.t;
			return *this;
		}

		//Accessors:

		key_compare key_comp() const { return  t.key_comp(); }
		value_compare value_comp()const{ return t.value_compare(t.key_comp());  }

		iterator begin() { return t.begin(); }
		iterator end() { return t.end(); }
		const_iterator begin() const { return t.begin(); }
		const_iterator end() const { return t.end(); }

		/*reverse_iterator rbegin() { return t.rbegin(); }
		reverse_iterator rend() { return t.rend(); }
		const_reverse_iterator rbegin() const { return t.rbegin(); }
		const_reverse_iterator rend() const { return t.rend(); }*/

		bool empty() const { return t.empty(); }
		size_type size() const { return t.size(); }
		size_type max_size() const { return t.max_size(); }
		T& operator[](const key_type& k){
			return (*((_insert(value_type(k, T()))).first)).second;
		}
		void swap(map<Key, T, Compare>& x){ t.swap(x.t); }

		pair<iterator, bool> insert(const value_type& val){
			return t.insert_unique(x);
		}
		iterator insert(iterator position, const value_type& x){
			return t.insert_unique(position, x);
		}


		void erase(iterator positon){ t.erase(positon); }
		size_type erase(const key_type& k){ return t.erase(k); }
		void erase(iterator first, iterator last){ t.erase(first,last); }
		void clear(){ t.clear(); }

		iterator find(const key_type& x){ return t.find(x);  }
		const_iterator find(const key_type& x) const { return t.find(x); }

		size_type count(const key_type& x) const{ return t.count(x); }
		iterator lower_bound(const key_type& x) { return t.lower_bound(x); }
		const_iterator lower_bound(const key_type& x)const { return t.lower_bound(x); }
		iterator upper_bound(const key_type& x){ return t.upper_bound(x); }
		const_iterator upper_bound(const key_type& x) const { return t.upper_bound(x); }

		pair<iterator, iterator> equal_range(const key_type& k){
			return t.equal_range(k);
		}

		pair<const_iterator, const_iterator> equal_range(const key_type& k) const {
			return t.equal_range(k);
		}

		friend bool operator == (const map&, const map&);
		friend bool operator<(const map&, const map&);
	
	};

	template<class Key,class Value,class KeyOfValue,class Compare>
	inline bool operator==(const map<Key, Value, KeyOfValue, Compare>&x, 
		const map<Key, Value, KeyOfValue, Compare>& y){
		return x.t == y.t;
	}

	template<class Key, class Value, class KeyOfValue, class Compare>
	inline bool operator<(const map<Key, Value, KeyOfValue, Compare>&x,
		const map<Key, Value, KeyOfValue, Compare>& y)
	{
		return x.t < y.t;
	}

}


#endif