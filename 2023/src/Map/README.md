**定义**：map是STL的一个关联容器，它提供一对一（key-value）的数据处理能力。其中key为关键字，每个关键字只能在map中出现一次，value为该关键字的值。

键可以是基本类型，也可以是类类型。  

map内部自建一颗红黑树，这颗树具有对数据自动排序的功能，所以在map内部所有的数据都是有序的。  
- map是关联容器，它按照特定的次序(按照key来比较)存储由键值key和值value组合而成的元素。
- 在map中，键值key通常用于排序和惟一地标识元素，而值value中存储与此键值key关联的内容
- 在内部，map中的元素总是按照键值key进行比较排序的。
- map中通过键值访问单个元素的速度通常比unordered_map容器慢，但map允许根据顺序对元素进行直接迭代(即对map中的元素进行迭代时，可以得到一个有序的序列)。
- map支持下标访问符，即在[]中放入key，就可以找到与key对应的value。
- map通常被实现为二叉搜索树(更准确的说：平衡二叉搜索树(红黑树))(map之所以效率高,是因为其内部采用了平衡二叉搜索树这种对数据具有快速排序和查找特性的数据结构,特别是红黑树这种平衡性能很好的自平衡二叉搜索树。这保证了map在大量数据情况下仍然可以维持很好的查找和操作效率。)


# Map的用法  
## 元素访问（读操作）
&emsp; 对map进行**读**操作的手法有：[ ]，at(since c++11)  

1：当查找的键不存在时,[ ]会自动创建一个新元素并插入,而at会抛出out_of_range异常。

2：at比[ ]更安全,它会进行边界检查,如果索引越界则抛异常。[ ]不会进行边界检查。

3：at相对于[ ]有**轻微的性能损失**,因为它需要进行边界检查。

4：当要访问的值可能不存在时,如果不想插入新元素,应该使用**find**而不是[ ]。

5：at可以通过异常处理获取更明确的错误信息。[ ]不能指示错误的原因。

6：**at主要用于访问已知存在的元素,[ ]可用于访问可能不存在的元素。**

7：at只读,不会插入新元素。[]可读写,会插入新元素。

&emsp;所以简单来说:

1：如果要安全访问已知存在的键,使用at。

2：如果要访问可能不存在的键,并在不存在时插入新键,使用[]。

3：如果不想在不存在时插入,应使用find。 

参考代码MapUse1.cpp
## 元素修改（insert erase swap clear emplace emplace_hint操作）
### insert
&emsp; 其实上面讲的[ ]也可以进行插入操作，它与insert的区别在于：[ ]会覆盖原有的值（如果key已经存在），而当insert试图插入的元素所对应的键已经在容器中，则insert 将不做任何操作。
```
// 假设map由string和int组成，以下例程来源于参考资料
// 1) Assignment using array index notation
Foo["Bar"] = 12345;

// 2) Assignment using member function insert() and STL pair
Foo.insert(std::pair<string,int>("Bar", 12345));

// 3) Assignment using member function insert() and "value_type()"
Foo.insert(map<string,int>::value_type("Bar", 12345));

// 4) Assignment using member function insert() and "make_pair()"
Foo.insert(std::make_pair("Bar", 12345));

```
从上面的4个例子里面，需要强调以下几点：
* std::map<std::string, int>::value_type就是 std::pair<std::string const, int>，所以2）和3）基本一样  
* 在 C++11 之后,使用 make_pair() 要比 value_type() 更高效。它可以避免额外的对象构造和析构开销。当然,这种效率差异也不是很大。但为了获取最佳性能,插入map时推荐使用 make_pair() 来创建键值对。  
### erase
-- Removes from the map container either a single element or a range of elements  
* erase(key)，删除key指定的元素，并重新排序。返回删除的元素个数，如果不存在该key指定的元素，则返回0
* erase(iter)，删除迭代器位置的元素
* erase(strt_iter, end_iter) ，删除起始和终止范围内所有元素  
参考代码 MapUse2.cpp    

&emsp;**erase使用的注意事项**
```
#include <map>
#include <iostream>

int main() {
    std::map<std::string,std::string> mapTest;  
    typedef std::map<std::string,std::string>::iterator ITER;  
    for(ITER iter=mapTest.begin();iter!=mapTest.end();++iter)  {  
        std::cout<<iter->first<<":"<<iter->second<<std::endl;  
        mapTest.erase(iter);  
    }
    return 0;
}
```
这是一种错误的写法，会导致程序行为不可知，原因是map是关联容器，对于关联容器来说，如果一个元素已经被删除，那么其对应的迭代器就失效了（erase后,原来的iter就变成了无效迭代器），不应该再被使用，但是循环体继续使用了这个无效的iter进行++操作，这属于未定义行为

改为正确用法一：（since c++11）   
**erase() 成员函数返回下一个元素的迭代器**

```
#include <map>
#include <iostream>
 
int main()
{
    std::map<int, std::string> c =
    {
        {1, "one"}, {2, "two"}, {3, "three"},
        {4, "four"}, {5, "five"}, {6, "six"}
    };
 
    // erase all odd numbers from c
    for (auto it = c.begin(); it != c.end();)
    {
        if (it->first % 2 != 0)
            it = c.erase(it);//  在循环体中, 将it更新
        else
            ++it;
    }
 
    for (auto& p : c)
        std::cout << p.second << ' ';
    std::cout << '\n';
}
```

正确用法二：
```
#include <map>
#include <iostream>
 
int main()
{
    std::map<int, std::string> c =
    {
        {1, "one"}, {2, "two"}, {3, "three"},
        {4, "four"}, {5, "five"}, {6, "six"}
    };
 
    // erase all odd numbers from c
    for (auto it = c.begin(); it != c.end();)
    {
        if (it->first % 2 != 0)
            c.erase(it++);//  在erase之后,使用了it++,将it更新为erase返回的下一个位置。
        else
            ++it;
    }
 
    for (auto& p : c)
        std::cout << p.second << ' ';
    std::cout << '\n';
}
```
### swap
交换两个map中的元素  
eg:  
```
// swap maps
#include <iostream>
#include <map>

int main ()
{
  std::map<char,int> foo,bar;

  foo['x']=100;
  foo['y']=200;

  bar['a']=11;
  bar['b']=22;
  bar['c']=33;

  foo.swap(bar);

  std::cout << "foo contains:\n";
  for (std::map<char,int>::iterator it=foo.begin(); it!=foo.end(); ++it)
    std::cout << it->first << " => " << it->second << '\n';

  std::cout << "bar contains:\n";
  for (std::map<char,int>::iterator it=bar.begin(); it!=bar.end(); ++it)
    std::cout << it->first << " => " << it->second << '\n';

  return 0;
}
```
### clear
将map中的元素清空  
### emplace
&emsp;Inserts a new element into the container constructed in-place with the given args if there is no element with the key in the container.
&emsp;Careful use of emplace allows the new element to be constructed while avoiding unnecessary copy or move operations. The constructor of the new element (i.e. std::pair<const Key, T>) is called with exactly the same arguments as supplied to emplace, forwarded via std::forward<Args>(args).... The element may be constructed even if there already is an element with the key in the container, in which case the newly constructed element will be destroyed immediately.  
**emplace vs insert in C++ STL**  
&emsp;In C++, all containers (vector, stack, queue, set, map, etc) support both insert and emplace operations.
&emsp;Both are used to add an element in the container.
The advantage of emplace is, it does in-place insertion and avoids an unnecessary copy of object. For primitive data types, it does not matter which one we use. But for objects, use of emplace() is preferred for efficiency reasons.  

&emsp;emplace更**高效 efficiency**
```
// C++ code to demonstrate difference between
// emplace and insert
#include<bits/stdc++.h>
using namespace std;
  
int main()
{
    // declaring map
    multiset<pair<char, int>> ms;
      
    // using emplace() to insert pair in-place
    ms.emplace('a', 24);
      
    // Below line would not compile
    // ms.insert('b', 25);    
      
    // using insert() to insert pair in-place
    ms.insert(make_pair('b', 25));    
      
    // printing the multiset
    for (auto it = ms.begin(); it != ms.end(); ++it)
        cout << " " << (*it).first << " "
             << (*it).second << endl;
  
    return 0;
}
```
### emplace_hint
emplace_hint()函数用于通过使用提示作为元素位置将新元素插入到容器中来扩展map容器。元素是直接构建的（既不复制也不移动）。  

与emplce相比：
* emplace_hint() 可以在指定位置就地构造插入,效率更高。

* emplace() 会自动选择位置插入,使用更简单。

* emplace_hint() 需要注意位置合法性,否则插入会失败。

## Iterators迭代器（）
## 查询（find--Searches the container for an element with a key equivalent to k and returns an iterator to it if found, otherwise it returns an iterator to map::end.)

map::find()是C++ STL中的内置函数，该函数返回一个迭代器或常量迭代器，该迭代器或常量迭代器引用键在映射中的位置。如果键不存在于Map容器中，则它返回引用map.end()的迭代器或常量迭代器。  
参考代码MapUse3.cpp

