[TOC]

### 一、list概述

#### 1、笔记整理原则

（1）尽量画很多图来加深对list数据结构和设计的理解，**一图胜千言**。

（2）不会详细讲解所有的代码，而是讲解**代码中的关键点，关键的数据结构和关键操作，并且对一些关键的边界条件进行讲解。**

（1）尽量通过画图的方式讲解代码和设计，一图胜千言，这样能让读者以更加直观的方式理解代码和容器的一些相关设计。

#### 2、list简单特性

​        stl的list容器底层使用双向链表的形式来存储用户需要存储的各种数据，这些数据可以是c++已有的数据类型，例如int、double、float等等，也可以是用户使用struct自定义结构的数据。这种可以让用户自行选择需要处理或者存储的数据结构，主要使用了c++ 的template。

​	list相比与vector与array申请连续的内存空间，list每次只申请一个node节点大小的内存空间，然后填入数据，再把它连入双向链表中。下表是list、vector、array的对比

|                      | list                                   | vector                                                       | array    |
| -------------------- | -------------------------------------- | ------------------------------------------------------------ | -------- |
| 申请内存类型         | 离散内存块，并且以双向链表形式进行组织 | 连续空间                                                     | 连续空间 |
| 是否可以动态扩充     | 是，每次扩充一个node大小               | 是，当插入操作不够内存的时候，连续空间以两倍增长，并且需要移动到新的内存位置 | 否       |
| 是否可以使用全局sort | 否                                     | 是                                                           | 是       |
| 插入和删除时间复杂度 | o(1)                                   | o(n)                                                         | o(n)     |
| 随机访问时间复杂度   | o（n）                                 | o(1)                                                         | o(1)     |

#### 3、list主要数据结构和设计概述

list里面主要有三种主要的类设计，包括list_node、list_iterator和list。

​	list_node主要用来把用户传进来的数据，在内存中组织成双向链表的结构；

​	list_iterator（迭代器）里面包含了一个指向list_node节点的指针和一大堆的操作符重载，例如前++、后++、前--、后--、指针操作符->、*、==、!=所有涉及到指针的操作，它都进行重载。它的设计初衷就是设计成类似指针的操作，所有对list_iterator的对象操作符操作，都转化为类内对list_node指针的操作。例如获取节点数据（用户存储的数据）、指针++等等。

​	list里面包括了一个list_node的指针，还有对list_node的一系列操作，包括节点插入、begin（获取初始节点的iterator）、end（获取结束节点的iterator）、rbegin、rend、swap等等，本质的思想也都是透过list_node指针，内存中的list_node组织的数据进行操作。其中，m_node -> next指向begin节点，m_node指向end节点。

### 二、list_node、list_iterator和list

#### 1、list_node

​	list_node是一个双向链表的数据结构，也是list容器底层用来管理用户数据的结构类型。它的主要代码和数据结构如下图所示：

![](./picture/list_2.jpg)

#### 2、list_iterator

​	list_iterator模仿的是一种list容器的指针行为，容器在操作内存中管理的list_node或者算法需要处理list_node节点组织的数据的时候，都是通过list_iterator作为中间桥梁的方式进行的。

​	所以，从本质上来说，list_iterator要设计成一个类，这个类拥有一个list_node指针，可以对list_node内容进行操作，同时它需要重载操作符前++、后++、前--、后--、*、->、==、!=等操作来让它表现出来的像是一个指针的操作。下面是list_iterator的实现代码:

```cpp
template<calss T, class Ref, class Ptr>
struct __list_iterator{

	typedef __list_iterator<T,T&,T*> iterator;
	typedef __list_iterator<T,Ref,Ptr> self;
	typedef bidirectional_iterator_tag iterator_category;
	typedef T value_type;
    typedef Ptr pointer;
    typedef Ref reference;
    typedef __list_node<T>* link_type;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    link_type node;    
    // contructor
    __list_iterator(link_type x):node(x){}
    __list_iterator():(){}
    __list_iterator(const iterator x):node(x.node){}    
    // operator
    // 比较两个iteraotr，比较的是两个iterator的node是否相等
    bool operator == (const self &x) const
    {
    	return node == x.node;
    }
    bool operator != (const self &x) const
    {
    	return !(node != x.node)
    }
    reference operator *() const
    {
    	return (*node).data;
    }
    pointer operator -> () const
    {
    	return &(operator*());
    }
    self & operator ++()	//前++，例如a++
    {
    	node = (link_type)node->next;
    	return *this;
    }
    self operator ++ (int)	//这里返回的是临时变量，而不是地址，因为返回的那一行，node不变
    {
    	self tmp = *this;
    	node = node->next;
    	return tmp;
    }
    self &operator--()
    {
    	node = (link_type)node->prev;
    	return *this;
    }
    self operator -- (int)	//这里返回的是临时变量，而不是地址，因为返回的那一行，node不变
    {
    	self tmp = *this;
    	node = node->prev;
    	return tmp;
    }
    

};
```

#### 3、list数据结构

​	list里面有一个指向双向链表节点的指针node，然后通过iterator对内存中的双向链表进行操作，它有一系列的操作，后面再讲，这里主要讲list的数据结构和双向链表形式。

（1）list有一个指向list_node节点的指针

（2）通过iterator对内存中的双向链表进行操作

（3）list容器符合前闭后开的设计，最终的数据组织形式是双向链表，并且是环状的，收尾相连。node->next指向begin，node指向end。

![](./picture/list_3.jpg)

![](./picture/list_4.jpg)

​	

### 三、list的关键操作

#### 1、涉及到内存分配与释放的操作：get_node/put_node/create_node/destroy_node

![](./picture/list_5.jpg)

#### 2、list开始初始化一个节点和insert操作

（1）list容器初始化的时候会创建一个开始节点，里面没有任何的数据，同时node/prev/next都指向本身

![](./picture/list_6.jpg)

（2）list的insert就是在双向链表中插入一个中间节点，这个好理解

![](./picture/list_7.jpg)

#### 3、其他元素操作：push_front/push_back/erase/pop_front/pop_back/clear/remove/unique/splice/merge/reverse/sort

这里只选一些比较简单的函数

![](./picture/list_8.jpg)

![](./picture/list_9.jpg)

![](./picture/list_10.jpg)

![](./picture/list_11.jpg)

