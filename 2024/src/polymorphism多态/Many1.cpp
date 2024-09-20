#include <iostream>

class AbstractSingleton {
public:
    virtual void SomeMethod(); //{ /* 应该提供一个默认实现，或者什么都不写 */ }
};

class SingletonImpl : public AbstractSingleton {
public:
    void SomeMethod()  override {std::cout<<"SingletonImpl"<<std::endl;};

};
class Vir{
public:
	virtual void tryVirtual() {};// 没有{}就报错
};
 
class CVir:public Vir{
public:
	void tryVirtual(){
		std::cout<<"CVir"<<std::endl;
	}
};

int main(){
    CVir cr;
    cr.tryVirtual();
    Vir vir;
    vir.tryVirtual();
    SingletonImpl sli; //编译报错
    sli.SomeMethod();

    return 0;
}