[TOC]

### 一、vector概述

#### 1、笔记整理原则

（1）尽量画很多图来加深对list数据结构和设计的理解，**一图胜千言**。

（2）不会详细讲解所有的代码，而是讲解**代码中的关键点，关键的数据结构和关键操作，并且对一些关键的边界条件进行讲解。**

（3）只讲解关键点，让你能完全hold住的关键点。

#### 2、vector简单介绍

（1）stl的vector本质上来说就是一个可以自动进行内存扩充的动态数组，当内存不够的时候，会自动进行两倍内存的扩充。

（2）vector并不是原地进行扩充，它在内存扩充的时候。会在新的内存位置申请两倍大小的内存，然后把原来的元素一一移动过来。

 下表是list、vector、array的对比

|                      | list                                   | vector                                                       | array    |
| -------------------- | -------------------------------------- | ------------------------------------------------------------ | -------- |
| 申请内存类型         | 离散内存块，并且以双向链表形式进行组织 | 连续空间                                                     | 连续空间 |
| 是否可以动态扩充     | 是，每次扩充一个node大小               | 是，当插入操作不够内存的时候，连续空间以两倍增长，并且需要移动到新的内存位置 | 否       |
| 是否可以使用全局sort | 否                                     | 是                                                           | 是       |
| 插入和删除时间复杂度 | o(1)                                   | o(n)                                                         | o(n)     |
| 随机访问时间复杂度   | o（n）                                 | o(1)                                                         | o(1)     |

#### 3、vector主要数据结构和设计概述

vector里面主要有两种主要的类设计，包括iterator、vector。

​	因为vector维护的是连续的线性内存空间，它的主要工作就是把用户需要的数据在连续的线性空间上进行管理。

​	正因为vector管理的是连续的线性空间，所以它的iterator直接就是用户指定数据结构的指针，指针满足iterator的所有需要的操作，++/--/*/->/==/!=等等，此外，vector还有连续空间所拥有的随机存取的功能。

​	vector包括主要的数据结构和相关的操作，主要的数据结构有iterator类型的start、end、end_of_storage，分别用来标识连续空间的开始、有效数据的结束位置和连续空间的结束位置。其他的操作也非常简单，后面会详细叙述。

​	还有一个需要注意的地方，空间扩充不是在原地扩充，而是在新的地方以两倍大小进行扩充，并且一旦发生扩充，原有的vector的迭代器都将失效。

### 二、iterator和vector实现

#### 1、iterator

​	正因为vector管理的是连续的线性空间，所以它的iterator直接就是用户指定数据结构的指针，指针满足iterator的所有需要的操作，++/--/*/->/==/!=等等，此外，vector还有连续空间所拥有的随机存取的功能。

![](./picture/list_12.jpg)

#### 2、vector

​	vector包括主要的数据结构和相关的操作，主要的数据结构有iterator类型的start、end、end_of_storage，分别用来标识连续空间的开始、有效数据的结束位置和连续空间的结束位置。其他的操作也非常简单，后面会详细叙述。

​	为了减少分配内存的速度成本，vector在分配内存的时候会分配多一点，以备未来的不时之需，这就是vector的capcitpy容量的由来。

![](./picture/list_13.jpg)

![](./picture/list_14.jpg)

​	



​	运用start、finish、end_of_storage三个迭代器，就可以轻易提供首尾表示、大小、容量、空容器等判断、随机存取等功能。

![](./picture/list_15.jpg)

### 三、vector相关操作

#### 1、vector的构造与内存管理

（1）vector提供很多constructors，其中一个允许我们指定空间大小和初值。

```cpp
vector(size_type n, const T& value)
{
    fill_initialize(n,value);
}
// 填充并且初始化
void fill_initialize(size_type n, const T& value)
{
    start = allocate_and_fill(n,value);
    finish = start + n;
    end_of_storage = finish;
}
// 配置后填充
iterator allocate_and_fill(size_type n,const T& x)
{
    iterator result = data_allocator::allocate(n);
    uninitialized_fill_n(result,n,x);
    return result;
}
```

​	当我们使用push_back把新的元素插入vector中的时候，该函数会首先检查是否有备用空间，如果有的话，就直接在备用空间上面构造元素，并且调整迭代器finish。如果没有就扩充空间（重新分配两倍大小空间、移动数据、释放原空间。

```cpp
void push_back(const T & x)
{
    if(finish != end_of_storage)
    {
        construct(finish,x);
        ++finish;	//调整水位高度
    }
    else
        insert_aux(end(),x);
}
template<class T,class Alloc>
void vector<T,Alloc>::insert_aux(iterator position, const T&x)
{
    if(finish != end_of_storage)
    {
    	construct(finish, *(finish-1));
    	++finish;
    	T x_copy = x;
    	copy_backward(position, finish-1, finish-1);
    	*position = x_copy;
    }
    else	//已经没有备用空间
    {
        const size_type old_size = size();
        const size_type len = old_size != 0 ? 2* old_size :1;
        iterator new_start = data_allocator::allocate(len);
        iterator new_finish = new_start;
        try
        {
            // 把原先vector内容拷贝到新的vector
            new_finish = uninitialized_copy(start,position,new_start);
            // 为新的元素赋初试x
            construct(new_finish,x);
            ++ new_finish;
            new_finish = uninitialized_copy(position ,finish,new_finish);
        }
        catch(...)
        {
            destroy(new_start,new_finish);
            data_allocator::deallocate(new_start,len);
            throw;	//抛出异常，这里省略代码
        }
        //释放原来的vector
        destroy(begin(),end());
        deallocate();
        
    }
}
```



#### 2、vector的元素操作：pop_back/erase/clear/insert

![](./picture/list_16.jpg)

![](./picture/list_17.jpg)

insert代码实现：

```cpp
template<class T,class Alloc>
void vector<T,Alloc>::insert(iterator position,size_type n,const T&x)
{
    if(n!=0)
    {
        // 如果空间够放
        if(size_type(end_of_storage-finish) >= n)
        {
            T x_copy = x;
            const size_type elems_after = finish - position;
            iterator old_finish = finish;
			//尽量减少元素移动
            if(elems_after >n)
            {
                uninitialized_copy(finish-n,finish,finish);
                finish += n;
                copy_backward(position,position +n,x_copy);
            }
            else
            {
                uninitialized_fill_n(finish,n-elems_after,x_copy);
                finish += n - elems_after;
                uninitialized_copy(position, old_finish,finish);
                finish += elems_after;
                fill(position ,old_finish,x_copy);
            }
        }
        else	//备用空间小于新增元素个数，需要扩充
        {
            const size_type old_size = size();
            cosnt size_type len = old_size + max(old_size,n);
            // 配置新的空间
            iterator newnew_start = data_allocator::allocate(len);
        	iterator new_finish = new_start;
        	__STL_TRY
        	{
            	new_finish = uninitialized_copy(start ,position,new_start);
            	new_finish =  uninitialized_fill_n(new_finish,n,x);
           		new_finish = uninitialized_copy(position ,finish,new_finish);
        	}
        	catch
        	{
         		//代码省略   
        	}
            destroy(start,finish);
            deallocate();
        	start = new_start;
            finish = new_finish;
            end_of_storage = start + len;
        }  
    }
}

```



#### 3、相关的测试程序在仓库的stl_test里面，里面涉及到所有容器的测试。

