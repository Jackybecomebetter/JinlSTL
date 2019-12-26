[TOC]

### 一、set和multiset概述

#### 1、笔记整理原则

（1）尽量画很多图来加深对list数据结构和设计的理解，**一图胜千言**。

（2）不会详细讲解所有的代码，而是讲解**代码中的关键点，关键的数据结构和关键操作，并且对一些关键的边界条件进行讲解。**

（3）只讲解关键点，让你能完全hold住的关键点。

#### 2、set和multiset简单介绍

（1）stl的set底层是用红黑树实现的，红黑树本身就是一颗二叉搜索树，所以对它来说，本身的元素就是自动进行排序的。header的左孩子节点指向了key值最小的节点，右孩子节点指向了key值最大节点。每次iterator进行++的时候，都会自动定位到下一个排序的key值所在的节点。

（2）set本质就是集合，只有键值，没有value值。set不允许两个元素具有相同的键值。同时，因为set底层是红黑树，一旦对里面元素的key值进行修改的话，会改变红黑树的构造，所以set不支持修改。

（3）set的所有对元素的构造和操作都是通过底层的红黑树进行实现的。

（4）multiset的特性与用法与set完全相同，唯一的差别是它允许键值重复。multiset的底层也是用红黑树实现的。

### 二、set关键源代码

```cpp
template<class Key,class Compare= less<Key>,class Alloc=alloc>
class set{
    typedef Key key_type;
    typedef Key value_type;
    //以下的key_compare与value_compare使用同一个比较函数
    typedef Compare key_compare;
    typedef Compare value_compare;
private:
	typedef rb_tree<key_type,value_type,identity<value_type>,key_compare,Alloc> rep_type;
	rep_type t;	//内部使用的红黑树
	...
	//全部都是用红黑树进行操作，没什么可看的
	key_compare key_comp() const {return t.key_comp();}
	value_compare value_comp() const {return t.key_comp();}
	iterator begin() cnst {return t.begin();}
	iterator end() const {return t.end();}
	reverse_iterator rbegin() const {retur t.rbegin();}
	reverse_iterator rend() const {return t.rend();}
	...
};
```



### 三、multiset关键源代码

multiset其他的源代码跟set差不多，底层也是声明了一个红黑树对象，然后通过红黑树对象实现对元素的操作。其中不同的是multiset支持插入相同的key值的元素，它调用的是insert_equal()。

