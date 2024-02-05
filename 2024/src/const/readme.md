# const在成员函数用法
## const再函数名之后--> 常量成员函数
把 const 放在成员函数的参数列表闭括号之后，返回类型之前，来指定它是一个常量成员函数。  
例如：  
```
class MyClass {
public:
    int getValue() const { // 正确用法
        return m_value;
    }
private:
    int m_value;
};
```  
在这个用法中，const 关键字在成员函数的参数列表之后。这表明 getValue 是一个**常量成员函数**，它不会修改MyClass类的任何普通的成员变量，并且只能调用其他的常量成员函数。详情参考用例：c1.cpp  

## 常量成员函数重载：  
两个成员函数，名字和参数表都一样，但是一个是const，一个不是，那么算重载。  
```
class Sample
{
public:
    Sample() { m_value = 1; }
    int GetValue() const { return m_value; } // 常量成员函数
    int GetValue() { return 2*m_value; } // 普通成员函数
    int m_value;
};

int main()
{
    const Sample obj1;// 常量对象，定义该对象的时候在前面加const关键字
    std::cout << "常量成员函数 " << obj1.GetValue() << std::endl;

    Sample obj2;
    std::cout << "普通成员函数 " << obj2.GetValue() << std::endl;
}

// 输出
常量成员函数 1
普通成员函数 2
```
## const 在成员函数名之前
保护指针指向的内容或引用的内容不被修改，即你不能通过这个指针来改变指向的值，但是你可以将这个指针指向别处。  
例子：如果函数要返回一个对象的引用，而且你希望调用者不能通过这个引用修改对象，那么使用 const 就合适了：
```
class MyClass {
public:
    //第一个const: 函数返回一个常对象引用，类型为 const MyClass&。这种返回类型的一个初衷通常是为了防止别处对此返回对象的修改，防止该对象的数据被误改。
    //第二个const：不能在这个函数内部修改类的任何数据成员，即它不会改变对象的状态。
    const MyClass& getAnotherInstance() const {
        return *this; // 返回对当前实例的const引用
    }
};
```
# const与非成员函数
如果是非成员函数，函数的返回类型前面或函数体内不能有 const 关键字。此类情况下的 const 只能出现在指针或引用返回类型的后面，来表明返回的数据不可修改：
```
const int* myFunction(); // 返回一个指向const int的指针
const int& myFunction(); // 返回一个对const int的引用
int myFunction() const;  // 错误写法，非成员函数不能使用
```

class MyClass {
public:
    const  int getValue() { 
        return m_value;
    }
private:
    int m_value;
};

在这个示例中，getValue 成员函数的返回类型是 const int，这意味着函数返回的整数值不应该被修改。然而，因为整数是返回值类型，它实际上返回的是一个副本，函数调用者并没有修改原始值的能力。因此，在这种情况下，const 关键字并没有实际的效果，因为返回的是值而非引用或指针。

# 总结一下  
如下：  
1.   
如果成员函数返回了一个对象的副本（如基本数据类型或对象），就不需要在返回类型前面加 const，因为副本是局部的，它的改变不会影响原始值。如果返回的是对原始对象的引用或指针，并且你想防止函数的调用者通过该引用或指针修改对象，那么就应该在引用或指针类型后面加上 const。  
2.   
①修饰形参：`int fun(const int a)`   

a在函数里不可被修改；  

②修饰返回值：`const int* fun( )`    

函数返回的**指针或者是引用**，加const规定返回值不可被修改；  

③函数后加const：`int fun( ) const`  

这个函数不能访问类中所有this所能调用的内存，即这是个**只读函数**；类的成员函数后面加了const关键字，说明这个函数是不能改变类中的成员变量的；如果在编写该ocnst成员函数时，不慎修改了数据成员，或者调用了其他非const成员函数，编译器将指出错误，这就提高了代码健壮   
3.   
关于“指针常量”还是“指向常量的指针”，看到一个小tips：    
从右往左读，遇到变量p就替换成"p is a"遇到"*"就替换成"point to"     
`int const* p`读作：p is a point to const int, 指向常量的指针； [**说明**：`int const* p`和`const int* p`在C++中表示完全相同的东西。都是表示p是一个指针，它指向一个常整数，你不能通过这个指针来改变这个整数的值。]   
`int * const p`读作：p is a const point to int, 常量指针。  