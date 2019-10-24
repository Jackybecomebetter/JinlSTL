#include <array>
#include <iostream>
#include <ctime>
#include <cstdlib>
using namespace std;

#define ASIZE 500000

// tool function
long get_a_target_long()
{
long target = 0;
    cout << "target (0 ~ " << RAND_MAX << ")";
    cout << "please input a target you want to find" << endl;
    cin >> target;
    return target;
}

string get_a_target_string()
{
long target = 0;
char buf[10];
    cout << "target (0 ~ " << RAND_MAX << ")";
    cout << "please input a target you want to find" << endl;
    cin >> target;
    snprintf(buf,10,"%d" , target);
    return string(buf);
    
}

int compreLongs(const void* a , const void *b)
{
    return ( *(long*)a - *(long*)b );
}

int compareStrings(const void *a , const void *b)
{
    if( *(string*)a > *(string*)b )
        return 1;
    else if( *(string *)a < *(string*)b )
        return -1;
    else
    {
        return 0;
    }
    
}

// hlx01 : 用来测试array 容器的插入和查找性能
// 具体做法：
// 1、随机生成500000个int，然后插入array里面
// 2、显示花费时间、array大小、第一个元素、最后一个元素、初始地址
// 3、排序，然后使用二分查找法查找某个元素。    显示排序和查找时间
namespace hlx01
{

void test_array()
{
    cout << "test array ....." << endl;

array<long,ASIZE> c;

clock_t timestart = clock();
    for(long i=0; i < ASIZE ; ++i)
    {
        c[i] = rand() / 100000;
    }
    cout  << "array.size(): " << c.size() << endl;
    cout << "array.front(): " << c.front() << endl;
    cout << "array.back(): " << c.back() << endl;
    cout << "array.data(): " << c.data() << endl;

    cout << "insert cost milli seconds : " << (clock() - timestart) * 1000.00 / CLOCKS_PER_SEC  << endl;

long target = get_a_target_long();

    timestart = clock();
    qsort(c.data(),ASIZE,sizeof(long),compreLongs);
    cout << "qsort cost milli seconds : " << (clock() - timestart) * 1000.00 / CLOCKS_PER_SEC  << endl;
    timestart = clock();
long * pItem = (long*)bsearch(&target, (c.data()), ASIZE , sizeof(long),compreLongs);
    cout << "bsearch cost milliseconds : " << (clock() - timestart) * 1000.00 / CLOCKS_PER_SEC  << endl;

    if(pItem != NULL)
        cout << "found " << endl;
    else
    {
        cout << "not found" << endl;
    }

}

}

namespace hlx02
{
    
}

int main()
{
    // test array
    hlx01::test_array();
}
