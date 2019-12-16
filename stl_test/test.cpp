#include <array>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <algorithm>
using namespace std;

#define ASIZE 500000

// 测试说明：
// 从hlx01 到 hlx05都是测试序列式容器：array vector、list、forward_list deque，测试插入、查找和排序效率 
// 从hlx06 到 hlx0 都是测试关联式容器：multiset、multimap、 插入和查找效率（关联式本身的存储方式不支持排序的功能，所以没有测试排序）

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

#include<vector>
#include<stdexcept>
#include<string>
#include<cstdlib>
#include<cstdio>
#include<iostream>
#include<ctime>
#include<algorithm>

// 测试vector插入、查找和排序
namespace hlx02
{

void test_vector(long value)
{
    cout << "start to test vector "<< endl;

vector<string> c;
char buf[10];

clock_t timestart = clock();
    for(long i=0 ; i < value ; ++i)
    {
        try
        {
            snprintf(buf,10,"%d",rand() / 100000);
            c.push_back(buf);
        }
        // 如果内存不够的话，可以检测到push哪个的时候发生错误
        catch(exception &p)
        {
            cout << "i = " << i << " " << p.what() << endl;
            abort();
        }
    }
    cout << "milli-seconds " << (clock() - timestart) * 1000.00 / CLOCKS_PER_SEC  << endl;
    cout << "vector.size() = " << c.size() << endl;
    cout << "vector.front() = " << c.front() << endl;
    cout << "vector.back() = " << c.back() << endl;
    cout << "vector.data() = " << c.data() << endl;
    cout << "vector.capacity() = " << c.capacity() << endl;

string target = get_a_target_string();
    {
timestart = clock();
auto pItem = ::find(c.begin(),c.end(),target);
    cout << "::find(), milli-seconds: " << (clock() - timestart) * 1000.00 / CLOCKS_PER_SEC  << endl;

    if(pItem != c.end())
        cout  << "found " << *pItem << endl;
    else
        cout << "not found" << endl;
    }
    {
    timestart = clock();
    sort(c.begin(),c.end());
string *pItem = (string*)bsearch(&target,(c.data()),c.size(),sizeof(string),compareStrings);
    cout << "sort then bsearch cost milli-seconds : " << (clock() - timestart) * 1000.00 / CLOCKS_PER_SEC << endl;

    if(pItem != NULL)
        cout << "found " << *pItem << endl;
    else
        cout << "not found" << endl;
    }

}

}

// 测试list的查找和排序
#include<list>
namespace hlx03
{

void test_list(long value)
{
    cout << "start to test list "<< endl;

list<string> c;
char buf[10];

clock_t timestart = clock();
    for(long i=0 ; i < value ; ++i)
    {
        try
        {
            snprintf(buf,10,"%d",rand() / 100000);
            c.push_back(buf);
        }
        // 如果内存不够的话，可以检测到push哪个的时候发生错误
        catch(exception &p)
        {
            cout << "i = " << i << " " << p.what() << endl;
            abort();
        }
    }
    cout << "milli-seconds " << (clock() - timestart) * 1000.00 / CLOCKS_PER_SEC  << endl;
    cout << "list.size() = " << c.size() << endl;
    cout << "list.maxsize() = " << c.max_size() << endl;
    cout << "list.front() = " << c.front() << endl;
    cout << "list.back() = " << c.back() << endl;

string target = get_a_target_string();
    {
timestart = clock();
auto pItem = ::find(c.begin(),c.end(),target);
    cout << "::find(), milli-seconds: " << (clock() - timestart) * 1000.00 / CLOCKS_PER_SEC  << endl;

    if(pItem != c.end())
        cout  << "found " << *pItem << endl;
    else
        cout << "not found" << endl;
    }
    {
    timestart = clock();
    c.sort();
    cout << "sort cost milli-seconds : " << (clock() - timestart) * 1000.00 / CLOCKS_PER_SEC << endl;
    }

}

}

// 测试forward list的查找和排序
#include<forward_list>
namespace hlx04
{

void test_forwardlist(long value)
{
    cout << "start to test forward list "<< endl;

forward_list<string> c;
char buf[10];

clock_t timestart = clock();
    for(long i=0 ; i < value ; ++i)
    {
        try
        {
            snprintf(buf,10,"%d",rand() / 100000);
            c.push_front(buf);  //单向链表只能从头部插入
        }
        // 如果内存不够的话，可以检测到push哪个的时候发生错误
        catch(exception &p)
        {
            cout << "i = " << i << " " << p.what() << endl;
            abort();
        }
    }
    cout << "milli-seconds " << (clock() - timestart) * 1000.00 / CLOCKS_PER_SEC  << endl;
    cout << "forwardlist.maxsize() = " << c.max_size() << endl;
    cout << "forwardlist.front() = " << c.front() << endl;

string target = get_a_target_string();
    {
timestart = clock();
auto pItem = ::find(c.begin(),c.end(),target);
    cout << "::find(), milli-seconds: " << (clock() - timestart) * 1000.00 / CLOCKS_PER_SEC  << endl;

    if(pItem != c.end())
        cout  << "found " << *pItem << endl;
    else
        cout << "not found" << endl;
    }
    {
    timestart = clock();
    c.sort();
    cout << "sort cost milli-seconds : " << (clock() - timestart) * 1000.00 / CLOCKS_PER_SEC << endl;
    }

}

}

// 测试deque的查找和排序
#include<deque>
namespace hlx05
{

void test_deque(long value)
{
    cout << "start to test deque "<< endl;

deque<string> c;
char buf[10];

clock_t timestart = clock();
    for(long i=0 ; i < value ; ++i)
    {
        try
        {
            snprintf(buf,10,"%d",rand() / 100000);
            c.push_front(buf);  //单向链表只能从头部插入
        }
        // 如果内存不够的话，可以检测到push哪个的时候发生错误
        catch(exception &p)
        {
            cout << "i = " << i << " " << p.what() << endl;
            abort();
        }
    }
    cout << "milli-seconds " << (clock() - timestart) * 1000.00 / CLOCKS_PER_SEC  << endl;
    cout << "deque.size = " << c.size() << endl;
    cout << "deque.front() = " << c.front() << endl;
    cout << "deque.back() = " << c.back() << endl;
    cout << "deque.maxsize() = " << c.max_size() << endl;

string target = get_a_target_string();
    {
timestart = clock();
auto pItem = ::find(c.begin(),c.end(),target);
    cout << "::find(), milli-seconds: " << (clock() - timestart) * 1000.00 / CLOCKS_PER_SEC  << endl;

    if(pItem != c.end())
        cout  << "found " << *pItem << endl;
    else
        cout << "not found" << endl;
    }
    {
    timestart = clock();
    sort(c.begin(),c.end());
    cout << "sort cost milli-seconds : " << (clock() - timestart) * 1000.00 / CLOCKS_PER_SEC << endl;
    }

}

}

// 测试multiset的插入效率
// 测试使用全局查找的方法和multiset本身查找的效率对比
#include<set>
namespace hlx06
{

void test_multiset(long value)
{
    cout << "start to test multiset "<< endl;

multiset<string> c;
char buf[10];

clock_t timestart = clock();
    for(long i=0 ; i < value ; ++i)
    {
        try
        {
            snprintf(buf,10,"%d",rand() / 100000);
            c.insert(buf);  //单向链表只能从头部插入
        }
        // 如果内存不够的话，可以检测到push哪个的时候发生错误
        catch(exception &p)
        {
            cout << "i = " << i << " " << p.what() << endl;
            abort();
        }
    }
    cout << "multiset insert cost milli-seconds " << (clock() - timestart) * 1000.00 / CLOCKS_PER_SEC  << endl;
    cout << "multiset.size = " << c.size() << endl;
    cout << "multiset.maxsize() = " << c.max_size() << endl;

string target = get_a_target_string();
    {
timestart = clock();
auto pItem = ::find(c.begin(),c.end(),target);
    cout << "::find(), milli-seconds: " << (clock() - timestart) * 1000.00 / CLOCKS_PER_SEC  << endl;

    if(pItem != c.end())
        cout  << "found " << *pItem << endl;
    else
        cout << "not found" << endl;
    }

    {
timestart = clock();
auto pItem = c.find(target);
    cout << "::find(), milli-seconds: " << (clock() - timestart) * 1000.00 / CLOCKS_PER_SEC  << endl;

    if(pItem != c.end())
        cout  << "found " << *pItem << endl;
    else
        cout << "not found" << endl;
    }

}

}

// 测试multimap的插入效率
// 测试multimap本身查找的效率对比
#include<map>
namespace hlx07
{

void test_multiset(long value)
{
    cout << "start to test multiset "<< endl;

multimap<long,string> c;
char buf[10];

clock_t timestart = clock();
    for(long i=0 ; i < value ; ++i)
    {
        try
        {
            snprintf(buf,10,"%d",rand() / 100000);
            c.insert(pair<long,string>(i,buf));  //单向链表只能从头部插入
        }
        // 如果内存不够的话，可以检测到push哪个的时候发生错误
        catch(exception &p)
        {
            cout << "i = " << i << " " << p.what() << endl;
            abort();
        }
    }
    cout << "multimap insert cost milli-seconds " << (clock() - timestart) * 1000.00 / CLOCKS_PER_SEC  << endl;
    cout << "multimap.size = " << c.size() << endl;
    cout << "multimap.maxsize() = " << c.max_size() << endl;

long target = get_a_target_long();

    {
timestart = clock();
auto pItem = c.find(target);
    cout << "multiset.find(), milli-seconds: " << (clock() - timestart) * 1000.00 / CLOCKS_PER_SEC  << endl;

    if(pItem != c.end())
        cout  << "found " << (*pItem).second << endl;
    else
        cout << "not found" << endl;
    }

}

}


// 测试unordered_multiset的插入效率
// 测试unordered_multiset本身查找的效率对比
#include<unordered_set>
namespace hlx08
{

void test_unorder_multiset(long value)
{
    cout << "start to test unorder multiset "<< endl;

unordered_multiset<string> c;
char buf[10];

clock_t timestart = clock();
    for(long i=0 ; i < value ; ++i)
    {
        try
        {
            snprintf(buf,10,"%d",rand() / 100000);
            c.insert(string(buf));  //单向链表只能从头部插入
        }
        // 如果内存不够的话，可以检测到push哪个的时候发生错误
        catch(exception &p)
        {
            cout << "i = " << i << " " << p.what() << endl;
            abort();
        }
    }
    cout << "multimap insert cost milli-seconds " << (clock() - timestart) * 1000.00 / CLOCKS_PER_SEC  << endl;
    cout << "unordered multiset.size() = " << c.size() << endl;
    cout << "unordered multiset.maxsize() = " << c.max_size() << endl;
    cout << "unordered multiset.bucket_count() = " << c.bucket_count() << endl;
    cout << "unordered multiset.load_factor() = " << c.load_factor() << endl;
    cout << "unordered multiset.max_load_factor = " << c.max_load_factor() << endl;
    cout << "unordered multiset.max_bucket_count() = " << c.max_bucket_count() << endl;

    for(unsigned i=0; i < 20; ++i)
    {
        cout << "bucket # " << i << "has" << c.bucket_size(i) << " elements." << endl;
    }

string target = get_a_target_string();

    {
timestart = clock();
auto pItem = ::find(c.begin(),c.end(),target);
    cout << "::find(), milli-seconds: " << (clock() - timestart) * 1000.00 / CLOCKS_PER_SEC  << endl;

    if(pItem != c.end())
        cout  << "found " << *pItem << endl;
    else
        cout << "not found" << endl;
    }

    {
timestart = clock();
auto pItem = c.find(target);
    cout << "unordered_multiset.find(), milli-seconds: " << (clock() - timestart) * 1000.00 / CLOCKS_PER_SEC  << endl;

    if(pItem != c.end())
        cout  << "found " << *pItem << endl;
    else
        cout << "not found" << endl;
    }

}

}

// 测试unordered_multimap的插入效率
// 测试unordered_multimap本身查找的效率对比
#include<unordered_map>
namespace hlx09
{

void test_unorder_multiset(long value)
{
    cout << "start to test unordered multimap "<< endl;

unordered_map<long,string> c;
char buf[10];

clock_t timestart = clock();
    for(long i=0 ; i < value ; ++i)
    {
        try
        {
            snprintf(buf,10,"%d",rand() / 100000);
            c.insert(pair<long,string>(i,string(buf)));  //单向链表只能从头部插入
        }
        // 如果内存不够的话，可以检测到push哪个的时候发生错误
        catch(exception &p)
        {
            cout << "i = " << i << " " << p.what() << endl;
            abort();
        }
    }
    cout << "unordered multimap insert cost milli-seconds " << (clock() - timestart) * 1000.00 / CLOCKS_PER_SEC  << endl;
    cout << "unordered multimap.size() = " << c.size() << endl;
    cout << "unordered multimap.maxsize() = " << c.max_size() << endl;
    cout << "unordered multimap.bucket_count() = " << c.bucket_count() << endl;
    cout << "unordered multimap.load_factor() = " << c.load_factor() << endl;
    cout << "unordered multimap.max_load_factor = " << c.max_load_factor() << endl;
    cout << "unordered multimap.max_bucket_count() = " << c.max_bucket_count() << endl;

    for(unsigned i=0; i < 20; ++i)
    {
        cout << "bucket # " << i << "has" << c.bucket_size(i) << " elements." << endl;
    }

long target = get_a_target_long();

    {
timestart = clock();
auto pItem = c.find(target);
    cout << "unordered_multimap.find(), milli-seconds: " << (clock() - timestart) * 1000.00 / CLOCKS_PER_SEC  << endl;

    if(pItem != c.end())
        cout  << "found " << (*pItem).second << endl;
    else
        cout << "not found" << endl;
    }

}

}

// 测试set的插入效率
// 测试set本身查找的效率和全局查找函数的查找效率对比
#include<unordered_map>
namespace hlx10
{

void test_set(long value)
{
    cout << "start to test set "<< endl;

set<string> c;
char buf[10];

clock_t timestart = clock();
    for(long i=0 ; i < value ; ++i)
    {
        try
        {
            snprintf(buf,10,"%d",rand() / 100000);
            c.insert(string(buf));  //单向链表只能从头部插入
        }
        // 如果内存不够的话，可以检测到push哪个的时候发生错误
        catch(exception &p)
        {
            cout << "i = " << i << " " << p.what() << endl;
            abort();
        }
    }
    cout << "set insert cost milli-seconds " << (clock() - timestart) * 1000.00 / CLOCKS_PER_SEC  << endl;
    cout << "set.size() = " << c.size() << endl;
    cout << "set.maxsize() = " << c.max_size() << endl;

string target = get_a_target_string();

    {
timestart = clock();
auto pItem = ::find(c.begin(),c.end(),target);
    cout << "::find(), milli-seconds: " << (clock() - timestart) * 1000.00 / CLOCKS_PER_SEC  << endl;

    if(pItem != c.end())
        cout  << "found " << *pItem << endl;
    else
        cout << "not found" << endl;
    }

    {
timestart = clock();
auto pItem = c.find(target);
    cout << "set.find(), milli-seconds: " << (clock() - timestart) * 1000.00 / CLOCKS_PER_SEC  << endl;

    if(pItem != c.end())
        cout  << "found " << *pItem << endl;
    else
        cout << "not found" << endl;
    }

}

}


// 测试map的插入效率
// 测试map本身查找的效率
#include<map>
namespace hlx11
{

void test_map(long value)
{
    cout << "start to test map "<< endl;

map<long,string> c;
char buf[10];

clock_t timestart = clock();
    for(long i=0 ; i < value ; ++i)
    {
        try
        {
            snprintf(buf,10,"%d",rand() / 100000);
            c.insert(pair<long,string>(i,string(buf)));  //单向链表只能从头部插入
        }
        // 如果内存不够的话，可以检测到push哪个的时候发生错误
        catch(exception &p)
        {
            cout << "i = " << i << " " << p.what() << endl;
            abort();
        }
    }
    cout << "map insert cost milli-seconds " << (clock() - timestart) * 1000.00 / CLOCKS_PER_SEC  << endl;
    cout << "map.size() = " << c.size() << endl;
    cout << "map.maxsize() = " << c.max_size() << endl;

long target = get_a_target_long();

    {
timestart = clock();
auto pItem = c.find(target);
    cout << "map.find(), milli-seconds: " << (clock() - timestart) * 1000.00 / CLOCKS_PER_SEC  << endl;

    if(pItem != c.end())
        cout  << "found " << (*pItem).second << endl;
    else
        cout << "not found" << endl;
    }

}

}

// 测试unorder map的插入效率
// 测试unorder map本身查找的效率
#include<map>
namespace hlx12
{

void test_ordered_map(long value)
{
    cout << "start to test unordered map "<< endl;

unordered_map<long,string> c;
char buf[10];

clock_t timestart = clock();
    for(long i=0 ; i < value ; ++i)
    {
        try
        {
            snprintf(buf,10,"%d",rand() / 100000);
            c.insert(pair<long,string>(i,string(buf)));  //单向链表只能从头部插入
        }
        // 如果内存不够的话，可以检测到push哪个的时候发生错误
        catch(exception &p)
        {
            cout << "i = " << i << " " << p.what() << endl;
            abort();
        }
    }
    cout << "unordered map insert cost milli-seconds " << (clock() - timestart) * 1000.00 / CLOCKS_PER_SEC  << endl;
    cout << "unordered map.size() = " << c.size() << endl;
    cout << "unordered map.maxsize() = " << c.max_size() << endl;
    cout << "unordered map.bucket_count() = " << c.bucket_count() << endl;
    cout << "unordered map.max_bucket_count() = " << c.max_bucket_count() << endl;
    cout << "unordered map.max_load_factor() = " << c.max_load_factor() << endl;

long target = get_a_target_long();

    {
timestart = clock();
auto pItem = c.find(target);
    cout << "unordered map.find(), milli-seconds: " << (clock() - timestart) * 1000.00 / CLOCKS_PER_SEC  << endl;

    if(pItem != c.end())
        cout  << "found " << (*pItem).second << endl;
    else
        cout << "not found" << endl;
    }

}

}

// 测试unordered set的插入效率
// 测试unordered set本身查找的效率
#include<set>
namespace hlx13
{

void test_unordered_set(long value)
{
    cout << "start to test unordered set "<< endl;

unordered_set<string> c;
char buf[10];

clock_t timestart = clock();
    for(long i=0 ; i < value ; ++i)
    {
        try
        {
            snprintf(buf,10,"%d",rand() / 100000);
            c.insert(string(buf));  //单向链表只能从头部插入
        }
        // 如果内存不够的话，可以检测到push哪个的时候发生错误
        catch(exception &p)
        {
            cout << "i = " << i << " " << p.what() << endl;
            abort();
        }
    }
    cout << "unordered set insert cost milli-seconds " << (clock() - timestart) * 1000.00 / CLOCKS_PER_SEC  << endl;
    cout << "unordered set.size() = " << c.size() << endl;
    cout << "unordered set.maxsize() = " << c.max_size() << endl;
    cout << "unordered set.bucket_count() = " << c.bucket_count() << endl;
    cout << "unordered set.max_bucket_count() = " << c.max_bucket_count() << endl;
    cout << "unordered set.max_load_factor() = " << c.max_load_factor() << endl;

string target = get_a_target_string();

    {
timestart = clock();
auto pItem = ::find(c.begin(),c.end(),target);
    cout << "::find(), milli-seconds: " << (clock() - timestart) * 1000.00 / CLOCKS_PER_SEC  << endl;

    if(pItem != c.end())
        cout  << "found " << *pItem << endl;
    else
        cout << "not found" << endl;
    }

    {
timestart = clock();
auto pItem = c.find(target);
    cout << "unordered_set.find(), milli-seconds: " << (clock() - timestart) * 1000.00 / CLOCKS_PER_SEC  << endl;

    if(pItem != c.end())
        cout  << "found " << *pItem << endl;
    else
        cout << "not found" << endl;
    }

}

}

int main()
{
    // test array
    // hlx01::test_array();
    // hlx02::test_vector(1000000);
    // hlx03::test_list(1000000);
    // hlx04::test_forwardlist(1000000);
    // hlx05::test_deque(1000000);
    // hlx06::test_multiset(1000000);
    // hlx07::test_multiset(1000000);
    // hlx08::test_unorder_multiset(1000000);
    // hlx09::test_unorder_multiset(1000000);
    // hlx10::test_set(1000000);
    // hlx11::test_map(1000000);
    // hlx12::test_ordered_map(1000000);
    hlx13::test_unordered_set(1000000);

}
