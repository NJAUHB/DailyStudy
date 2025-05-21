/*
*这是一个懒汉模式线程安全的单例示例
*为啥安全？
*因为加锁了！
*https://juejin.cn/post/6844903928497176584?searchId=20231108175103F6D56480532D24C4A09A#heading-5
*/
#include <iostream>  // std::cout
#include <mutex>     // std::mutex
#include <thread>    //std::thread
#include <vector>

class SingleInstance {
 public:
  // 获取单例对象
  static SingleInstance* GetInstance();

  // 释放单例，进程退出时调用
  static void deleteInstance();

  // 打印单例地址
  void Print();

 private:
  // 将其构造和析构成为私有的, 禁止外部构造和析构
  SingleInstance();
  ~SingleInstance();

  // 将其拷贝构造和赋值构造成为私有函数, 禁止外部拷贝和赋值
  SingleInstance(const SingleInstance& signal);
  const SingleInstance& operator=(const SingleInstance& signal);

 private:
  // 唯一单例对象指针
  static SingleInstance* m_SingleInstance;
  static std::mutex S_Mutex;
};

//初始化静态成员变量
SingleInstance* SingleInstance::m_SingleInstance = NULL;
std::mutex SingleInstance::S_Mutex;  // 初始化互斥量
SingleInstance* SingleInstance::GetInstance() {
  std::unique_lock<std::mutex> lock(SingleInstance::S_Mutex);
  if (m_SingleInstance == NULL) {
    m_SingleInstance = new (std::nothrow) SingleInstance();  // 没有加锁是线程不安全的，当线程并发时会创建多个实例
  }

  return m_SingleInstance;
}

void SingleInstance::deleteInstance() {
  if (m_SingleInstance) {
    std::cout << "Start Delete Instance !!!" << std::endl;
    delete m_SingleInstance;
    m_SingleInstance = NULL;
  }
}

void SingleInstance::Print() {
  std::cout << "我的实例内存地址是:" << this << std::endl;
}

SingleInstance::SingleInstance() {
  std::cout << "构造函数" << std::endl;
}

SingleInstance::~SingleInstance() {
  std::cout << "析构函数" << std::endl;
}
///////////////////  普通懒汉式实现 -- 线程不安全  //////////////////

// 线程函数
/*
void *PrintHello(void *threadid) {} 这个函数的返回类型为void *，
即它返回一个类型未指定的指针。输入参数void *threadid同样也是一个类型未指定的指针。

这种设计通常在处理线程或者函数指针、涉及到不同数据类型的通用处理程序中看到。
使用void *类型的参数和返回值，意味着这个函数可以接受不同类型的指针参数，并返回不同类型的指针。
*/
void* PrintHello(void* threadid) {
  // 对传入的参数进行强制类型转换，由无类型指针变为整形数指针，然后再读取
  int tid = *((int*)threadid);

  std::cout << "Hi, 我是线程 ID:[" << tid << "]" << std::endl;

  // 打印实例地址
  SingleInstance::GetInstance()->Print();
  return NULL;
}

#define NUM_THREADS 50  // 线程个数

int main(void) {
  std::vector<std::thread> threads;
  int indexes[NUM_THREADS] = {0};  // 用数组来保存i的值
  std::cout << "main() : 开始 ... " << std::endl;
  for (int i = 0; i < NUM_THREADS; i++) {
    indexes[i] = i;  //先保存i的值
    // 传入的时候必须强制转换为void* 类型，即无类型指针
    threads.emplace_back(PrintHello, (void*)&(indexes[i]));
  }
  for (auto& thread : threads)
    thread.join();
  // 手动释放单实例的资源
  SingleInstance::deleteInstance();
  std::cout << "main() : 结束! " << std::endl;

  return 0;
}
