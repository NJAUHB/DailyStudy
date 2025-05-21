/*内部静态变量的懒汉单例（C++11 线程安全）
*☆☆☆这个方式非常推荐☆☆☆，实现的代码最少
*/
#include <thread>
#include <vector>
#include <iostream>

class InstanceExample {
 public:
  static InstanceExample* get_instance();
  void Print();
  // 释放单例，进程退出时调用
  static void deleteInstance();

 private:
  InstanceExample();
  ~InstanceExample();
};
InstanceExample::InstanceExample() {
  std::cout << "构造函数" << this << std::endl;
}
InstanceExample::~InstanceExample() {
  std::cout << "析构函数" << this << std::endl;
}
inline void InstanceExample::Print() {
  std::cout << "我的实例内存地址是:" << this << std::endl;
}

/*static InstanceExample instance;
*在这种情况下，instance是一个局部静态对象，它在函数第一次被调用时进行初始化，
*并在程序结束时自动析构。它存储在栈中，当函数返回时不会被销毁。

*static InstanceExample* instance = new InstanceExample();
*这里，instance 是一个指向动态分配对象的指针。该对象在堆中创建，并且只能通过调用delete来显式销毁。
*如果没有显式销毁，它将一直存在直到程序结束。但是，销毁的责任需要由程序员来处理，如果忘记调用delete，会导致内存泄露。

*在大多数情况下，推荐使用第一种方式，因为这种方式可以由编译器管理对象的生命周期，不存在内存泄露的风险，
*且相对于动态分配内存，访问栈内存更快。
*/
InstanceExample* InstanceExample::get_instance() {
  // static InstanceExample* instance = new InstanceExample();
  static InstanceExample instance;
  return &instance;
}
void* PrintHello(void* threadid) {
  // 对传入的参数进行强制类型转换，由无类型指针变为整形数指针，然后再读取
  int tid = *((int*)threadid);

  std::cout << "Hi, 我是线程 ID:[" << tid << "]" << std::endl;

  // 打印实例地址
  InstanceExample::get_instance()->Print();
  return NULL;
}

#define NumberOfThreads 5  // 宏定义不需要加逗号
int main() {
  std::vector<std::thread> Threads;
  int index_i[NumberOfThreads] = {0};
  for (int i = 0; i < NumberOfThreads; i++) {
    index_i[i] = i;
    Threads.emplace_back(PrintHello, (void*)(&(index_i[i])));
  }
  for (auto& it : Threads)
    it.join();
  std::cout << "main() : 结束!!!" << std::endl;
  return 0;
}