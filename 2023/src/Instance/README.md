&emsp; 在软件系统中，经常有这样一些特殊的类，必须保证他们在系统中只存在一个实例，才能确保它们的逻辑正确性、以及良好的效率。所以得考虑如何绕过常规的构造器（不允许使用者new出一个对象），提供一种机制来保证一个类只有一个实例。  
**定义：** 保证一个类仅有一个实例，并提供一个该实例的**全局**访问点
# 单例模式分类
&emsp; 单例模式可以分为懒汉式和饿汉式，两者之间的区别在于创建实例的时间不同：   
* 懒汉式：指系统运行中，实例并不存在，只有当需要使用该实例时，才会去创建并使用实例。（这种方式要考虑线程安全）  
* 饿汉式：指系统一运行，就初始化创建实例，当需要时，直接调用即可。（本身就线程安全，没有多线程的问题）  
&emsp; 想重点说明一下：在**C++11标准**以后，局部静态变量的初始化被保证为线程安全，所以我们可以很容易实现一个线程安全的单例类。这也不需要我们自己去造轮子确保线程安全勒！ 如下面示例： 
```
class Foo
{
public:
    static Foo *getInstance()
    {
        static Foo s_instance;
        return &s_instance;
    }
private:
    Foo() {}
};
```

# 单例类特点

- 构造函数和析构函数为private类型，目的禁止外部构造和析构  
- 删除 拷贝构造函数 和 拷贝赋值运算符，目的是禁止外部拷贝和赋值，确保实例的唯一性  
```
class Singleton {
public:
  // 公开的静态方法，用于获取单例对象的引用。
  static Singleton& getInstance() {
    // 静态局部变量只在第一次执行时初始化，以后不再初始化。
    static Singleton instance;  
    return instance;
  }

  Singleton(const Singleton&) = delete; // 这行代码阻止编译器自动生成拷贝构造函数。拷贝构造函数是类的一个特殊成员函数，它定义了一个新的对象如何从现有对象复制初始化。尝试进行复制初始化（比如 Singleton s1 = s2;）会导致编译错误。
  Singleton& operator=(const Singleton&) = delete;// 这行代码阻止编译器自动生成拷贝赋值运算符。拷贝赋值运算符是类的一个特殊成员函数，它定义了一个对象如何通过赋值语句从另一个同类对象复制状态。通过这行代码，我们禁止了这样的赋值，尝试进行拷贝赋值会导致编译错误。这是拷贝赋值运算符的典型声明方式。拷贝赋值运算符通常返回对 *this 的引用，使得连续赋值（如 a = b = c）成为可能

  // 一些其他成员，比如数据和方法...

private:
  // 私有构造函数，防止直接创建对象。
  Singleton() {
    // 初始化代码...
  }
};
```
- 类里有个获取实例的**静态成员函数，可以全局访问**  

# 知识点 
## 知识点1
局部静态变量：局部静态变量是在函数或者代码块内部定义的静态变量。它们在程序运行期间只会被初始化一次，而它的生命周期则贯穿整个程序的运行期间。 

静态成员变量特点和常见用法包括：  

对于静态成员变量，它们并不会在每个对象的构造过程中进行初始化，所以不能在构造函数的初始化列表中对静态成员变量进行初始化。因为它们只会被初始化一次（在类外进行），无论创建多少个类的对象，都只共享一个静态成员变量。
但我们是可以在构造函数体（或者类的其他成员函数）中赋值给静态成员变量。  
```
class Singleton {
private:
    // 私有的构造函数，禁止外部直接创建实例。
    Singleton() {}

    // 私有的拷贝构造函数和赋值操作符，防止被复制。
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

public:
    // 提供全局访问点
    static Singleton& getInstance() {
        // 局部静态变量的初始化是线程安全的
        static Singleton instance;
        return instance;
    }

    // 类的其他成员...
};
```
## 知识点2
单例模式下的getInstance方法通常被设为静态成员函数，原因是这样可以不依赖于类的任何具体实例就能调用该方法。这也正是静态成员函数的主要特点之一：它们属于类本身而不是类的某个对象实例。  
## 知识点3
静态成员函数的特点和常见用法包括：

类层次的函数：静态成员函数是类级别的，这意味着它们不依赖于类的实例。它们**不可以访问或修改对象的非静态成员**。

全局访问点：在单例模式中，getInstance提供了一个全局访问点，以便代码的任何部分都可以访问类的唯一实例。调用静态成员函数不需要先创建对象，可以直接通过类名来调用。

无需this指针：静态成员函数内部没有this指针，因为它们不是基于对象的。

存储持久性：静态成员函数只能够与类的静态成员变量互动(静态成员函数不能访问类的非静态成员变量，因为它不与类的任何对象关联。它只可以访问静态成员变量和其他静态成员函数)，这些变量的存储持久性与程序的生命周期一样长。  

静态成员函数不能被声明为虚函数。  

静态成员函数可以被继承，但不能被覆写。  

静态成员函数不能有 const 限定符：static member function cannot have cv-qualifierconst （成员函数是指不会修改其所在类的任何成员变量（除非那些成员变量被声明为 mutable）的函数。但是由于静态成员函数不操作类的实例，因此不需要 const 限定符。）  

静态成员函数定义的时候不可以加static关键字  
 error: cannot declare member function ‘static std::string& li::dsfdds::DomainParticipantFactory::get_dsf_version()’ to have static linkage [-fpermissive]
```
#include <iostream>

class MyClass {
private:
    static int myVar;  // 静态成员变量
public:
    static void staticMemberFunction() {  // 静态成员函数
        std::cout << "Value of static member variable myVar: " << myVar << std::endl;
    }
};

int MyClass::myVar = 10;  // 初始化静态成员变量

int main() {
    MyClass::staticMemberFunction();  // 调用静态成员函数
    return 0;
}
```