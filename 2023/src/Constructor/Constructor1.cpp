#include <iostream>

class Constructor {
 private:
  std::string name;

 public:
  Constructor(std::string name_) : name(name_) { std::cout << "My Name is " << name << std::endl; }
  void PrintName() { std::cout << "My Name is " << name << std::endl; }
};
int main() {
  std::string NAME1 = "hboyyyy";
  Constructor cc1(NAME1);
  Constructor cc2 = cc1;  //定义变量并初始化，调用默认拷贝构造函数
  Constructor cc3(cc2);   //定义变量并初始化，调用默认拷贝构造函数
  //cc2 = cc1; //用cc1来=号给cc2 编译器给我们提供的浅copy
  cc2.PrintName();
  cc3.PrintName();
  return 0;
}