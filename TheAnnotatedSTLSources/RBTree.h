#ifndef _RBTREE_H_
#define _RBTREE_H_


#include "Iterator.h"
#include <cstddef>
#include "utility.h"

namespace Jinl{

	//��ɫ���жϣ���ɫΪ0����ɫΪ1
	typedef bool rb_tree_color_type;
	const rb_tree_color_type rb_tree_red = false;
	const rb_tree_color_type rb_tree_black = true;

	//���ڵ�Ķ��壬���԰��� ��ɫ�����ڵ㡢���Һ��ӽڵ�
	struct rb_tree_node_base{
		typedef rb_tree_color_type color_type;
		typedef rb_tree_node_base* base_ptr;

		color_type color;
		base_ptr parent;
		base_ptr lchild;
		base_ptr rchild;

		//�����Сֵ
		static base_ptr minimum(base_ptr x){
			while (x->lchild != nullptr)
				x = x->lchild;
			return x;
		}
		//�����Сֵ
		static base_ptr maximum(base_ptr x){
			while (x->rchild != nullptr)
				x = x->rchild;
			return x;
		}

	};

	//�̳��Ի����Ľڵ�ṹ�������������ͽڵ�֮�����������
	template<class Value>
	struct rb_tree_node :public rb_tree_node_base
	{
		Value data;
		typedef rb_tree_node<Value>* link_type;
	};

	/*RB������������˫��ṹ��*/
	struct rb_tree_base_iterator{
		typedef rb_tree_node_base::base_ptr base_ptr;
		typedef BidirectionIterator_tag iterator_category;   //���������
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
			//����ǰ���Ϊheaderʱ�����ڸ��������ʱ��rchild�����ҵ������������������max���
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
	inline void rb_tree_balance(rb_tree_node_base* x, rb_tree_node_base*& root){
		x->color = rb_tree_red;
		while (x != root&&x->parent->color == rb_tree_red)
		{
			if (x->parent == x->parent->parent->lchild)
			{
				rb_tree_node_base* y = x->parent->parent->rchild;    //y��x�Ĳ����ڵ�
				if (y&&y->color == rb_tree_red){
					x->parent->color = rb_tree_black;
					y->color = rb_tree_black;
					x->parent->parent->color = rb_tree_red;
					x = x->parent->parent;
				}
				else{
					//����½ڵ�Ϊ���ڵ���Һ��ӣ��޲����ڵ�򲮸��ڵ�Ϊ��
					if (x == x->parent->rchild){
						x = x->parent;
						rb_tree_rotate_left(x, root);
					}
					x->parent->color = rb_tree_black;
					x->parent->parent->color = rb_tree_red;
					rb_tree_rotate_right(x->parent->parent, root);
				}
			}
			//���ڵ����游�ڵ������
			else{
				rb_tree_node_base* y = x->parent->parent->lchild;
				if (y&&y->color == rb_tree_red){
					x->parent->color = rb_tree_black;
					y->color = rb_tree_black;
					x->parent->parent->color = rb_tree_red;
					x = x->parent->parent;
				}
				//�޲����ڵ㣬�򲮸��ڵ�Ϊ��
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