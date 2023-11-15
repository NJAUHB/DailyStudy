#include <iostream>
#include <string>
using namespace std;
 
class Add
{
private:
    int x,y;
public:
    std::string z = "i am public!";
public:
    Add()
    {
        x=y=4;
    }
friend class Support; //类'Support'现在是类'Add'的朋友
};
 
class Support
{
public:
    void Sum(Add ob)//此函数可访问类'Add'的所有私有成员
    {
        cout<<"The sum of the 2 members is : "<<(ob.x+ob.y)<<endl;//不是友元类会报错
    }
};

class Base {
public:
    virtual void PrintMessage(const std::string& a) {
        std::cout << "Message from Base: " << a << std::endl;
    }
};

class Derived : public Base {
public:
    void PrintMessage() { // 注意参数列表与Base类一致
        std::cout << "Message from Derived"<< std::endl;
    }
};
 
int main()
{
    Add ad;
    std::cout << ad.z << std::endl;
    Support sup;
    sup.Sum(ad);
    auto ring = "hhh";
    Base* p_ = new Derived();
    Derived* p__ = new Derived();
    // p_ -> PrintMessage();//报错
    p__ -> PrintMessage();
    return 0;
}