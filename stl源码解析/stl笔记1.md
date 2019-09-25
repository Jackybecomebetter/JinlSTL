[TOC]



### 一、内存管理的简单介绍

#### 1、内存管理的层次

![](./图片/内存管理的层次.png)

​	内存管理的层次可以分为五个层次，c++ application（应用层）、c++ library（c++标准库，例如std::allocator）、c++ pimitives（c++基本单元，例如new new[] new() operator::new   ::operator::new）、CRT（malloc free）、OS API。

​	一层一层都是逐层封装的，对于上层的application来说，我们可以直接调用顶层的c++ library，也可以调用底下几层的API。

### 二、常见的c++内存分配的工具和调用流程图

#### 1、c++内存管理基本工具

![](./图片/c++内存管理的基本工具.png)

#### 2、new expression的调用流程

（1）调用operator new（操作符）分配内存，这个operator是可以被重载的

​	 调用malloc，成功就返回

​	 如果不成功，会进入_callnewh函数里面，可以释放一些内存，然后再重新调用malloc，直到分配内存成虫

（2）类型转换

（3）调用类的构造函数

![](./图片/new-expression调用流程.png)

#### 2、C++ delete expression调用流程

（1）调用析构函数

（2）调用operator delete释放内存

调用free释放内存

![](./图片/delete-expression调用流程.png)

#### 3、调用new expression申请数组内存（简析何时会发生内存泄漏）

​	调用delete [] ptr在释放对象指针对应的内存（整个数组的大小），还会调用每个数组里面对象对应的析构函数，所以如果析构函数里面有释放内存的话，调用delete ptr就会发生内存泄漏。

​	内存泄漏发生在：如果对象里面本身也有分配内存，并且分配的内存在析构函数进行释放。那么调用delete ptr相比于delete [] ptr就会发生内存泄漏。因为delete ptr本身只调用一次析构函数，然后再去释放对象对应内存。

​	当然，我们写代码的时候使用new array[n]，应该统一用delete [] array进行释放。

![](./图片/何时发生内存泄漏.png)

#### 4、placement new的用法和调用流程

![](./图片/placement-new用法.png)

![](./图片/placement-new调用流程.png)

#### 5、c++ malloc的内存分配图

![](./图片/c++malloc的内存分配图.png)

####  *6、C++应用程序使用new分配内存的流程

（1）一般重载类::operator new

（2）应用程序调用的new expression不可以重载

（3）::operator new（全局）可以重载，但一般不重载，因为他影响太多东西了

![](./图片/应用程序调用new的流程图.png)

#### *7、容器分配内存的流程图

![](./图片/容器分配内存的流程图.png)

### 三、对c++ primitives进行重载

####  1、重载全局的operator new（::operator new），一般很少用

![](./图片/重载全局的operator_new.png)

####  *2、类里面重载operator new和operator delete

![](./图片/类里面重载new.png)

#### *3、重载placement new

![](./图片/重载placement-new.png)

#### 4、Basic_string重载operator new（调用分配器分配内存）

![](./图片/basic_string重载new.png)

### 四、常见的编译器对分配内存的管理

#### 1、内存管理的目标

（1）减少调用malloc的次数，可以通过一次分配一大块内存，然后再根据使用者的需求去分配，不用每次都调用malloc（提高速度）。

（2）减少浪费（每次分配的内存有额外的cookie造成浪费），每次分配一大块内存，也可以降低内存浪费。

#### 2、改写类的operator new 和 delete(简单方式1)实现简单的内存分配

![](./图片/类改写new方式1.png)

![](./图片/类改写delete方式1.png)

#### 3、类改写new和delete简单方式2

![](./图片/类改写new方式2.png)

![](./图片/类改写delete方式2.png)

#### 4、为了减少不必要的重复工作，即对每个类都重写operator new和delete，我们可以写一个allocator类，让他来专门负责管理内存。

![](./图片/专门分配内存的allocator设计.png)

![](./图片/allocator用法.png)

#### *5、内存分配失败时候，new handler起到的作用和用法

​	C++在没能力为你分配你想申请的内存的时候，会抛出异常。在抛出异常之前，会进入一个new handler（人为设定），可以 释放一些内存，再重新申请

![](./图片/new-handler设计.png)

![](./图片/new-handler用法.png)

#### 6、Vc6的allocator没有做任何改进

![](./图片/vc6的allocator.png)

7、BC5的allocator没有做任何改进

![](./图片/bc5的allocator.png)

8、GC2.9的allocator没有做任何改进

![](./图片/gc2.9allocator.png)