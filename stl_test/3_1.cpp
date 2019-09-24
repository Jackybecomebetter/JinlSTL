#include<malloc.h>
#include<iostream>
// #include<allocator.h>
// #include"alloc.h"
#include "../TheAnnotatedSTLSources/alloc.h"

using namespace std;

template <class T>
class complex{
    T head;
    T tail;    
};

//  malloc/free、new、operator new、allocator用法

int main()
{
    void *p1 = malloc(512);
    free(p1);

    //c++表达式
    complex<int> *p2 = new complex<int>;
    delete p2;

    //c++函数
    void *p3 = ::operator new(512);
    ::operator delete(p3);

#ifdef _MSC_VER
    // 类直接家括号可以分配一个临时对象，当执行玩这一行之后，这个对象就被析构了
    int *p4 = allocator<int>().allocator(3,(int*)0);
    allocator<int>().deallocate(p4,3);
#endif
#ifdef __BORLANDC__
    int *p4 = allocator<int>().allocate(5);
    allocator<int>().deallocate(p4,5);
#endif
#ifdef __GNUC__
    void *p4 = Jinl::Alloc::allocate(512);
    Jinl::Alloc::deallocate(p4,512);
#endif

}

