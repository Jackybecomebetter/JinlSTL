[TOC]

### 一、deque概述

#### 1、笔记整理原则

（1）尽量画很多图来加深对list数据结构和设计的理解，**一图胜千言**。

（2）不会详细讲解所有的代码，而是讲解**代码中的关键点，关键的数据结构和关键操作，并且对一些关键的边界条件进行讲解。**

（3）只讲解关键点，让你能完全hold住的关键点。

#### 2、deque简单介绍

（1）stl的deque本质上来说就是一个分段连续的容器，它支持动态扩充，并且支持从容器头和容器尾进行插入和移除元素。这个分段连续的容器实际对用户表现出来的特性就是连续的内存空间。

（2）deque分段连续的设计是由中控器和iterator联合实现的，首先，中控器作为一个数组，里面存放指向不同连续缓冲区的指针，每当需要插入一个新的元素的时候，deque判断当前的缓冲区是否有多余的空间，如果没有的话，创建一个新的缓冲区，并且把该指针加入中控区进行管理。同时iterator仿照指针的行为，重载相关的操作符，对内存中的deque组织的数据结构 进行操作。

（3）deque对外表现为连续的内存空间，那么它也支持随机存取的操作。

#### 3、deque主要数据结构和设计概述

deque有三种关键的设计，包括deque的中控器、iterator、和deque数据结构。

![](./picture/deque_1.JPG)

​	deque中控器的本质是一个数组，里面存放指向不同连续缓冲区空间的指针，缓冲区存放着用户需要存储的数据。

​	iterator主要有四个关键的对象，其中cur、first、last指向当前缓冲区的当前元素、第一个元素和最后一个元素，还有一个指向中控器的指针map，通过它可以清楚地了解到当前指向哪一个缓冲区；同时为了让用户使用deque的时候就想使用普通的连续空间一样，iterator类内还对相关的操作符进行重载：++/--/+=/-=/*/->等等。

​	deque比较关键的对象由两个iterator，start和finish用于表示第一个节点和最后一个节点、map指向中控器，它是一个里面存放了指向缓冲区的指针，不同的缓冲区为连续的内存空间，用来组织和存储用户的数据、map_size用来表示map里面有多少指针，就是有多少缓冲区的意思。

### 二、中控器和iterator

#### 1、中控器

​	deque的最大任务就是把分段连续的空间，配置为用户所看到的的连续的空间。deque使用的是一个中控区，该中控器是一个数组，它里面存放着的元素都是指向缓冲区的指针。其中模拟指针的操作主要由iterator实现，内存分配和数据增删查改等操作主要由deque实现，后面会详细叙述。

![](./picture/deque_2.JPG)

#### 2、iterator

​	deque是分段连续空间，为了维持它整体连续的假象，具体的任务落在了迭代器的身上。迭代器主要通过对操作符进行重载来实现。

​	deque迭代器应该具备以下一些结构设计，首先它应该知道当前的缓冲区（分段连续空间）在哪里；其次，它必须能判断自己是否处在缓冲区的边缘，如果是的话，那么 前进或者后退，必须能跨越缓冲区。为了能成功跨越缓冲区，它必须知道中控器在哪里。

以下是iterator的具体实现：

```
template <class T,class Ref,class Ptr, size_t BufSiz>
struct __deque_iterator
{
    typedef __deque_iterator<T,T&,T*,BufSiz> iterator;
    typedef __deque_iterator<T, const T&,const T*,BufSiz> const_iterator;

	static size_t buffer_size() {return __deque_buf_size(BufSiz,sizeof(T));}	//返回		buffer_size大小，可以用户自行设置
	typedef random_access_iterator_tag iterator_category;
	typedef T value_type;
	typedef Ptr pointer;
	typedef Ref reference;
    typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef T** map_pointer;
    typedef __deque_iterator self;

    T*cur;	//指向当前缓冲区当前元素
    T* first;	//指向当前缓冲区的第一个元素
    T* last;	//指向当前缓冲区的最后一个元素
	map_pointer node;	//指向管控中心
...
}
```

当迭代器在行进的时候遇到边界的时候，需要缓冲区节点，同时更新first和last

```cpp
void set_node(map_pointer new_node)
{
	node = new_node;
	first = *new_node;
	last = first + difference_type(buffer_size());
}
```

以下是关于操作符重载

```cpp
	
```



#### 3、相的测试程序在仓库的stl_test里面，里面涉及到所有容器的测试。

