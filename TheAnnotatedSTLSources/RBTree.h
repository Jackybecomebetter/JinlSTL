#ifndef _RBTREE_H_
#define _RBTREE_H_


#include "Iterator.h"
#include <cstddef>
#include "utility.h"

namespace Jinl{

	//颜色的判断，红色为0，黑色为1
	typedef bool rb_tree_color_type;
	const rb_tree_color_type rb_tree_red = false;
	const rb_tree_color_type rb_tree_black = true;

	//树节点的定义，属性包括 颜色、父节点、左右孩子节点
	struct rb_tree_node_base{
		typedef rb_tree_color_type color_type;
		typedef rb_tree_node_base* base_ptr;

		color_type color;
		base_ptr parent;
		base_ptr lchild;
		base_ptr rchild;

		//获得最小值
		static base_ptr minimum(base_ptr x){
			while (x->lchild != nullptr)
				x = x->lchild;
			return x;
		}
		//获得最小值
		static base_ptr maximum(base_ptr x){
			while (x->rchild != nullptr)
				x = x->rchild;
			return x;
		}

	};

	//继承自基础的节点结构，添加了数据域和节点之间的链接类型
	template<class Value>
	struct rb_tree_node :public rb_tree_node_base
	{
		Value data;
		typedef rb_tree_node<Value>* link_type;
	};

	/*RB迭代器，采用双层结构。*/
	struct rb_tree_base_iterator{
		typedef rb_tree_node_base::base_ptr base_ptr;
		typedef BidirectionIterator_tag iterator_category;   //迭代器类别
		typedef ptrdiff_t               difference_type;

		base_ptr node;

		void increment(){
			if (node->rchild != nullptr){
				node = node->rchild;
				while (node->lchild != nullptr)
					node = node->lchild;
			}
			else{
				base_ptr y = node->parent;
				while (node == y->rchild){
					node = y;
					y = y->parent;
				}
				if (node->rchild != y)
					node = y;
			}
		}
		void decrement(){
			//当当前结点为header时，存在该情况。此时的rchild是最右的情况，既是整棵树的max结点
			if (node->color == rb_tree_red && node->parent->parent == node)
				node = node->rchild;
			else if (node->lchild != nullptr){
				base_ptr y = node->lchild;
				while (y->rchild != nullptr)
					y = y->rchild;
				node = y;
			}
			else{
				base_ptr y = node->parent;
				while (node == y->lchild){
					node = y;
					y = y->parent;
				}
				node = y;
			}
		}
	};


	template<class T, class Ref, class Ptr>
	struct rb_tree_iterator :public rb_tree_base_iterator{
		typedef rb_tree_node<T>* link_type;
		typedef Ref              reference;
		typedef Ptr              pointer;
		typedef T                value_type;

		typedef rb_tree_iterator<T, T&, T*>  iterator;
		typedef rb_tree_iterator<T, const T&, const T*> const_iterator;
		typedef rb_tree_iterator<T, Ref, Ptr>   self;

		rb_tree_iterator(){}
		rb_tree_iterator(link_type x) :node(x){}
		rb_tree_iterator(const iterator& x) :node(x.node){}

		reference operator*()const{ return link_type(node)->data; }
		pointer   operator->()const{ return &(operator*()); }

		self& operator++(){
			increment();
			return *this;
		}
		self operator++(int){
			self tmp = *this;
			increment();
			return tmp;
		}
		self& operator--(){
			decrement();
			return *this;
		}

		self operator--(int){
			self tmp = *this;
			decrement();
			return tmp;
		}

	};

	inline bool operator==(const rb_tree_base_iterator& x, const rb_tree_base_iterator& y){
		return x.node == y.node;
	}
	inline bool operator!=(const rb_tree_base_iterator& x, const rb_tree_base_iterator& y){
		return !(x == y);
	}

	inline BidirectionIterator_tag
		iterator_category(const rb_tree_base_iterator&){
		return BidirectionIterator_tag();
	}
	inline rb_tree_base_iterator::difference_type* 
		distance_type(const rb_tree_base_iterator&){
		return (rb_tree_base_iterator::difference_type *)0;
	}

	template <class T,class Ref,class Ptr>
	inline value_type* value_type(const rb_tree_iterator<T, Ref, Ptr>&){
		return (value_type*)0;
	}



	template<class Key, class Value, class KeyOfValue, class Compare>
	class rbTree{
	protected:
		typedef void*                  void_pointer;
		typedef rb_tree_node_base*     base_ptr;
		typedef rb_tree_node<Value>    rbTreeNode;
		typedef rb_tree_color_type	   color_type;
		typedef allocator<rbTreeNode>  rbTreeNodeAllocator;

	public:

		typedef  Key               key_type;
		typedef  Value             value_type;
		typedef  value_type*       pointer;
		typedef  const value_type* const_pointer;
		typedef  value_type&       reference;
		typedef  const value_type& const_reference;
		typedef  rb_tree_node*     link_type;
		typedef  size_t            size_type;
		typedef  ptrdiff_t         difference_type;

	protected:

		link_type getNode(){
			return rbTreeNodeAllocator::allocate();
		}
		void putNode(link_type p){
			rbTreeNodeAllocator::deallocate(p);
		}

		link_type createNode(const value_type& val){
			link_type tmp = getNode();
			construct(&tmp->data, val);
			return tmp;
		}

		link_type cloneNode(link_type x){
			link_type tmp = createNode(x->data);
			tmp->color = x->color;
			tmp->lchild = nullptr;
			tmp->rchild = nullptr;
			return tmp;
		}

		void destroyNode(link_type p){
			destroy(&p->data);
			putNode(p);
		}

	protected:
		size_type node_count;
		link_type header;
		Compare   key_compare;

		link_type& root() const{
			return (link_type&)header->parent;
		}
		link_type& leftmost() const{
			return(link_type&)header->lchild;
		}
		link_type& rightmost()const{
			return(link_type&)header->rchild;
		}

		static link_type& left(const link_type& x)
		{
			return (link_type&)(x->lchild);
		}
		static link_type& right(const link_type& x){
			return (link_type&)(x->rchild);
		}
		static link_type& parent(const link_type& x){
			return (link_type&)(x->parent);
		}
		static reference value(const link_type& x){
			return x->data;
		}

		static const Key& key(const link_type& x){
			return KeyOfValue()(value(x));
		}

		static color_type& color(const link_type& x){
			return (color_type&)(x->color);
		}

		static link_type& left(base_ptr x){
			return (link_type&)(x->lchild);
		}

		static link_type& right(base_ptr x){
			return (link_type&)(x->rchild);
		}

		static link_type& parent(base_ptr x){
			return (link_type&)(x->parent);
		}

		static reference value(base_ptr x){
			return ((link_type)x)->data;
		}
		static const Key& key(base_ptr x){
			return KeyOfValue()(value(link_type(x)));
		}
		static color_type& color(base_ptr x){
			return (color_type&)(link_type(x)->color);
		}

		static link_type minimum(const link_type& x){
			return (link_type)rb_tree_node_base::minimum(x);
		}

		static link_type maximum(const link_type& x){
			return (link_type)rb_tree_node_base::maximum(x);
		}

	public:

		typedef rb_tree_iterator<value_type, reference, pointer>   iterator;

	private:
		iterator _insert(base_ptr x, base_ptr y, const value_type& v);
		link_type _copy(link_type x, link_type p);
		void _erase(link_type x);
		void init(){
			header = getNode();
			color(header) = rb_tree_red;
			root() = 0;
			leftmost() = header;
			rightmost() = header;
		}

	public:
		rbTree(const Compare& comp = Compare()) :
			node_count(0), key_compare(comp){
			init();
		}
		~rbTree(){
			clear();
			putNode(header);
		}

		void clear(){
			if (node_count == 0){
				erase(root());
				leftmost() = header;
				root() = nullptr;
				rightmost() = header;
				node_count = 0;
			}
		}
		rbTree<Key, Value, KeyOfValue, Compare>&
			operator=(const rbTree<Key, Value, KeyOfValue, Compare>& x);

		Compare key_comp() const{
			return key_compare;
		}

		iterator begin(){
			return leftmost();
		}
		iterator end(){
			return header;
		}
		bool empty()const{
			return node_count == 0;
		}
		size_type size() const{
			return node_count;
		}

		size_type max_size()const{
			return size_type(-1);
		}

	public:
		template<class InputIterator>
		void insert_unique(InputIterator first, InputIterator last){
			for (; first != last; first++)
				insert_unique(*first);
		}
		size_type count(const key_type &x) const{
			pair<const_iterator, const_iterator> p = equal_range(k);
			size_type n = 0;
			distance(p.first, p.second, n);
			return n;
		}
		iterator lower_bound(const key_type &k){
			link_type y = header;
			link_type x = root();
			while (x != nullptr)
				if (key_comp(key(x), k))
					y = x; x = left(x);
				else
					x = right(x);

			return iterator(y);
		}
		const_iterator lower_bound(const key_type &k) const{
			link_type y = header;
			link_type x = root();
			while (x != nullptr)
				if (key_comp(key(x), k))
					y = x; x = left(x);
				else
					x = right(x);

			return iterator(y);
		}
		iterator upper_bound(const key_type &k){
			link_type y = header;
			link_type x = root();
			while (x != nullptr)
				if (key_comp(k, key(x)))
					y = x, x = left(x);
				else
					x = right(x);
			return iterator(y);
		}
		const_iterator upper_bound(const key_type &k) const{
			link_type y = header;
			link_type x = root();
			while (x != nullptr)
				if (key_comp(k, key(x)))
					y = x, x = left(x);
				else
					x = right(x);
			return iterator(y);
		}

		inline pair<const_iterator, const_iterator>::equal_range(const key& k) const {
			return pair<const_iterator, const_iterator>(lower_bound(k), upper_bound(k));
		}

		inline void erase(iterator position){
			link_type y = (link_type)rb_tree_rebalance_for_erase(
				position->node, header->parent, header->lchild, header->rchild);
			destroyNode(y);
			--node_count;
		}
		

		pair<iterator, bool> insert_unique(const value_type& val){
			link_type y = header;
			link_type x = root();

			bool comp = true;
			while (x != nullptr)
			{
				y = x;
				comp = key_comp(KeyOfValue()(val), key(x));
				x = comp ? left(x) : right(x);
			}

			iterator j = iterator(y);
			if (comp)
				if (j == begin())
					return pair<iterator, bool>(_insert(x, y, val), true);
				else j--;
				if (key_comp(key(j->node), KeyOfValue()(v)))
					return pair<iterator, bool>(_insert(x, y, val), true);
				return pair<iterator, bool>(j, false);
		}
		iterator insert_equal(const value_type& val){
			link_type y = header;
			link_type x = root();

			while (x != nullptr){
				y = x;
				x = key_compare(KeyOfValue()(val), key(x)) ? left(x), right(x);
			}
			return _insert(x, y, val);
		}
		iterator find(const Key& k);
	};

	template<class Key, class Value, class KeyOfValue, class Compare>
	typename rbTree<Key, Value, KeyOfValue, Compare>::iterator
		rbTree<Key, Value, KeyOfValue, Compare>::find(const Key& k){
		link_type y = header;
		link_type x = root();

		while (x != nullptr)
			if (!key_compare(key(x), k))
				y = x, x = left(x);
			else
				x = right(x);

		iterator j = iterator(y);
		return (j == end() || key_comp(k, key(j.node))) ? end() : j;
	}

	template<class Key, class Value, class KeyOfValue, class Compare>
	typename rbTree<Key, Value, KeyOfValue, Compare>::iterator
		rbTree<Key, Value, KeyOfValue, Compare>::_insert(base_ptr x_, base_ptr y_, const value_type& v)
	{
		link_type x = (link_type)x_;
		link_type y = (link_type)y_;

		link_type z;

		if (y == header || x != nullptr || key_comp(KeyOfValue()(v), key(y))){
			z = createNode(v);
			left(y) = z;
			if (y == header){
				root() = z;
				rightmost() = z;
			}
			else if (y == leftmost())
				leftmost() = z;
		}
		else{
			z = createNode(v);
			right(y) = z;
			if (y == rightmost())
				rightmost() = z;
		}
		parent(z) = y;
		left(z) = nullptr;
		right(z) = nullptr;
		rb_tree_balance(z, header->parent);
		++node_count;
		return iterator(z);

	}
	inline rb_tree_node_base* rb_tree_rebalance_for_erase(
		rb_tree_node_base* x, rb_tree_node_base*& root,
		rb_tree_node_base*& leftmost, rb_tree_node_base*& rightmost){

		rb_tree_node_base* y = x;
		rb_tree_node_base* z = nullptr;
		rb_tree_node_base* z_parent = nullptr;
		if (y->lchild == nullptr)
			z = y->rchild;
		else if (y->rchild == nullptr)
			z = y->lchild;
		else{
			y = y->rchild;
			while (y->lchild != nullptr)
				y = y->lchild;
			z = y->rchild;
		}
		if (y != x){
			x->lchild->parent = y;
			y->lchild = z->lchild;
			if (y != x->rchild)
			{
				z_parent = y->parent;
				if (z)
					z->parent = y->parent;
				y->parent->lchild = z;
				y->rchild = x->rchild;
				x->rchild->parent = y;
			}
			else
				z->parent = y;
			if (root = x)
				root = y;
			else if (x->parent->lchild == z)
				x->parent->lchild = y;
			else
				x->parent->rchild = y;
			y->parent = x->parent;
			swap(y->color, x->color);
			y = x;
		}
		else {
			//y == x
			z_parent = y->parent;
			if (z)
				z->parent = y->parent;
			if (root == x)
				root = z;
			else if (x->parent->lchild == x)
				x->parent->lchild = z;
			else
				x->parent->rchild = z;
			if (leftmost == x)
				if (x->rchild == nullptr)
					leftmost = x->parent;
				else
					leftmost = rb_tree_node_base::minimum(z);
			if (rightmost == x)
				if (x->lchild == nullptr)
					rightmost = x->parent;
				else rightmost = rb_tree_node_base::maximum(z);
		}

		if (y->color != rb_tree_red){
			while (z != root && (z == nullptr || z->color == rb_tree_black)){
				if (z == z_parent->lchild){
					rb_tree_node_base* w = z_parent->rchild;
					if (w->color == rb_tree_red){
						w->color = rb_tree_black;
						z_parent->color = rb_tree_red;
						rb_tree_rotate_left(z_parent, root);
						w = z_parent->rchild;
					}
					if ((w->lchild == nullptr || w->lchild->color == rb_tree_black) &&
						(w->rchild == nullptr || w->rchild->color == rb_tree_black)){
						w->rchild->color = rb_tree_red;
						z = z_parent;
						z_parent = z_parent->parent;
					} else {
						if (w->rchild == 0 || w->rchild->color == rb_tree_black)
							if (w->lchild)
								w->lchild->color = rb_tree_black;
						w->color = rb_tree_red;
						rb_tree_rotate_right(w, root);
						w = z_parent->rchild;
					}
					w->color = z_parent->color;
					z->parent->color = rb_tree_black;
					if (w->rchild)
						w->rchild->color = rb_tree_black;
					rb_tree_rotate_left(z_parent, root);
					break;
				}
				else{
					rb_tree_node_base* w = z_parent->lchild;
					if (w->color == rb_tree_red){
						w->color = rb_tree_black;
						z_parent->color = rb_tree_red;
						rb_tree_rotate_right(z_parent, root);
						w = z_parent->lchild;
					}
					if ((w->rchild == nullptr || w->rchild->color == rb_tree_black) &&
						(w->lchild == nullptr || w->lchild->color == rb_tree_black)){
						w->color = rb_tree_red;
						z = z_parent;
						z_parent = z_parent->parent;

					}
					else{
						if (w->lchild == nullptr || w->lchild->color == rb_tree_black){
							if (w->rchild)
								w->rchild->color = rb_tree_black;
							w->color = rb_tree_red;
							rb_tree_rotate_left(w, root);
							w = z_parent->lchild;
						}
						w->color = z_parent->color;
						z_parent->color = rb_tree_black;
						if (w->lchild)
							w->lchild->color = rb_tree_black;
						rb_tree_rotate_right(z_parent, root);
						break;
					}
				}
				if (z)
					z->color = rb_tree_black;
			}
		}
		return y;
		
	}
	inline void rb_tree_balance(rb_tree_node_base* x, rb_tree_node_base*& root){
		x->color = rb_tree_red;
		while (x != root&&x->parent->color == rb_tree_red)
		{
			if (x->parent == x->parent->parent->lchild)
			{
				rb_tree_node_base* y = x->parent->parent->rchild;    //y是x的伯父节点
				if (y&&y->color == rb_tree_red){
					x->parent->color = rb_tree_black;
					y->color = rb_tree_black;
					x->parent->parent->color = rb_tree_red;
					x = x->parent->parent;
				}
				else{
					//如果新节点为父节点的右孩子，无伯父节点或伯父节点为黑
					if (x == x->parent->rchild){
						x = x->parent;
						rb_tree_rotate_left(x, root);
					}
					x->parent->color = rb_tree_black;
					x->parent->parent->color = rb_tree_red;
					rb_tree_rotate_right(x->parent->parent, root);
				}
			}
			//父节点是祖父节点的左孩子
			else{
				rb_tree_node_base* y = x->parent->parent->lchild;
				if (y&&y->color == rb_tree_red){
					x->parent->color = rb_tree_black;
					y->color = rb_tree_black;
					x->parent->parent->color = rb_tree_red;
					x = x->parent->parent;
				}
				//无伯父节点，或伯父节点为黑
				else{
					if (x == x->parent->lchild){
						x = x->parent;
						rb_tree_rotate_right(x, root);
					}
					x->parent->color = rb_tree_black;
					x->parent->parent->color = rb_tree_red;
					rb_tree_rotate_left(x->parent->parent, root);
				}
			}
		}
		root->color = rb_tree_black;
	}
	inline void rb_tree_rotate_left(rb_tree_node_base* x, rb_tree_node_base*& root){
		rb_tree_node_base* y = x->rchild;
		x->rchild = y->lchild;
		if (y->lchild != 0)
			y->lchild->parent = x;
		y->parent = x->parent;

		if (x == root)
			root = y;
		else if (x == x->parent->lchild)
			x->parent->lchild = y;
		else
			x->parent->rchild = y;
		y->lchild = x;
		x->parent = y;
	}

	inline void rb_tree_rotate_right(rb_tree_node_base* x, rb_tree_node_base*& root){
		rb_tree_node_base* y = x->lchild;
		x->lchild = y->rchild;
		if (y->rchild != nullptr)
			y->rchild->parent = x;
		y->parent = x->parent;

		if (x == root)
			root = y;
		else if (x == x->parent->rchild)
			x->parent->rchild = y;
		else
			x->parent->lchild = y;
		y->rchild = x;
		x->parent = y;
	}




}



#endif