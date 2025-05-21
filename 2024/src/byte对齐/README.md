# 字节对齐
## 基本规则
- 成员对齐：结构体中第一个数据成员放在offset为0的地方，**以后每个成员相对于结构体首地址的偏移量都是 min（成员自身长度 , 默认对齐参数）的整数倍**，如有需要编译器会在成员之间加上填充字节。 
- 整体对齐：结构体的**总大小为结构体所用过的最大对齐参数的整数倍**，如有需要编译器会在最末一个成员之后加上填充字节。(结构体的总大小，也就是sizeof的结果，必须为所用过的最大对齐参数的整数倍，不够就补空字节。比如在32位系统中最大对齐参数是4，如果一个结构中有类型为int、long、float、double等长度等于或大于4的成员，则其总大小必须是4的整数倍。)
- 结构体作为成员：如果一个结构里有某些结构体成员，则结构体成员要从其**内部最大元素大小的整数倍地址**开始存储（嵌套结构体不能展开计算）。
- 默认对齐参数 = **其所有成员默认对齐要求的最大值**、
```c++
struct A {
    char a;   // 1字节对齐
    int b;    // 4字节对齐
    short c;  // 2字节对齐
}; // 结构体A的默认对齐方式是 4字节（成员中最大）

struct B {
    char a;      // 1字节对齐
    double b;    // 8字节对齐
}; // 结构体B的默认对齐方式是 8字节

```
## alignof
alignof的用法为：`alignof(类名)或者alignof(类对象)`  
首先，编译器会有一个默认的字节对齐数，记为N1(比较常见的是取8)。
假设结构体中每个成员内存长度为Li，max(Li)为最大的成员内存长度（对于结构体内有数组的情况，Li仍为单个数据类型元素的大小）。
如果未设置alignas，alignof返回的正是min(N1, max(Li))



## offsetof
c++ 结构体内存对齐 怎么看
在C++中，可以使用offsetof宏来检查结构体内成员的偏移量，从而了解内存对齐情况。offsetof宏定义在stddef.h头文件中，它返回结构体中成员的偏移量（以字节为单位）。

此外，可以使用sizeof运算符来获取整个结构体的大小，以此推断出数据对齐所使用的填充（padding）。

下面是一个示例代码，演示如何使用offsetof和sizeof来检查结构体内存对齐情况：
```c++
#include <iostream>
#include <cstddef>
#include <iostream>
 
struct MyStruct {
    char a;
    int b;
    char c;
};
 
int main() {
    std::cout << "Size of MyStruct: " << sizeof(MyStruct) << std::endl;
    std::cout << "Offset of 'a': " << offsetof(MyStruct, a) << std::endl;
    std::cout << "Offset of 'b': " << offsetof(MyStruct, b) << std::endl;
    std::cout << "Offset of 'c': " << offsetof(MyStruct, c) << std::endl;
    return 0;
}
```
运行这段代码，你将看到MyStruct结构体的大小以及各个成员的偏移量。根据结果，你可以推断出内存对齐的具体情况。

注意，编译器可以根据目标平台的对齐要求自由调整结构体的布局，可以通过编译器的特定指令或编译选项来指定或更改对齐方式，例如使用#pragma pack指令在Microsoft Visual C++中指定对齐边界。