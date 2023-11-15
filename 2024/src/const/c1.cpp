#include <iostream>
class MyClass {
private:
    // 基本类型（例如int, char, float等）必须初始化 不然没法实列化
    int normal_var=0;  // 普通变量
    static int static_var;  // 静态变量
    mutable int mutable_var=0;  // 可变变量
public:
    void const_func() const {
        // normal_var = 10;  // 错误：在常量成员函数内不能修改普通变量
        static_var = 20;  // 正确：在常量成员函数内可以修改静态变量
        mutable_var = 30;  // 正确：在常量成员函数内可以修改被mutable修饰的变量
        std::cout << static_var+mutable_var << std::endl;
    }
};
int MyClass::static_var = 0;  // 初始化静态成员变量

int main(){
    const MyClass mc0;
    MyClass mc1;
    mc0.const_func();
    mc1.const_func();
    return 0;
}