#include <iostream>
using namespace std;
class demo{
public:
   demo():num(new int(0)){
      cout<<"construct!"<<endl;
   }
   //拷贝构造函数
   demo(const demo &d):num(new int(*d.num)){
      cout<<"copy construct!"<<endl;
   }
   ~demo(){
      cout<<"class destruct!"<<endl;
   }
private:
   int *num;
};
/*执行 get_demo() 函数内部的 demo() 语句，即调用 demo 类的默认构造函数生成一个匿名对象；
*执行 return demo() 语句，会调用拷贝构造函数复制一份之前生成的匿名对象，并将其作为 get_demo() 函数的返回值（函数体执行完毕之前，匿名对象会被析构销毁）；
*执行 a = get_demo() 语句，再调用一次拷贝构造函数，将之前拷贝得到的临时对象复制给 a（此行代码执行完毕，get_demo() 函数返回的对象会被析构）；
*程序执行结束前，会自行调用 demo 类的析构函数销毁 a。
*----------------------------------------------------------------------------
*注意，目前多数编译器都会对程序中发生的拷贝操作进行优化g++ Constructor3.cpp -o c3
*./c3看到的往往是优化后的输出结果：
*construct!
*class destruct!
*----------------------------------------------------------------------------
*若取消优化g++ Constructor3.cpp -o c3 -fno-elide-constructors
*construct!            <-- 执行 demo()
*copy construct!       <-- 执行 return demo()
*class destruct!       <-- 销毁 demo() 产生的匿名对象
*copy construct!       <-- 执行 a = get_demo()
*class destruct!       <-- 销毁 get_demo() 返回的临时对象
*class destruct!       <-- 销毁 a
*/
demo get_demo(){
    return demo();
}
int main(){
    demo a = get_demo();
    return 0;
}