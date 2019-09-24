#include<malloc.h>

#include<iostream>
using namespace std;

class A
{
public:
    int id;

A() : id(0)
{
    cout << "default ctor.this" << this << "id = " << id << endl;
}
A(int i) : id(i)
{
    cout << "ctor.this" << this << "id= " << id << endl;
}
~A()
{
    cout << "dtor.this=" << this << "id = " << id << endl;
}

};

int main()
{
    A *buf = new A[3];
    A *tmp = buf;
    cout  << "buf is " << buf << "tmp is " << tmp << endl;

    for(int i=0; i < 3; i++)
        new(tmp++)A(i); //使用placement new调用构造函数
    
    delete[] buf;
}