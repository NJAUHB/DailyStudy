#include <iostream>
#include <thread>
#include <mutex>
#include <string>

std::once_flag flag;
const std::string& get_initialized_value() {
  /*静态局部变量只在第一次调用时初始化，并且在所有后续调用中保持其值。
    所有线程在调用 std::call_once 后都会访问相同的静态局部变量。*/
  static std::string value;
  std::call_once(flag, []() {
    std::cout << "Initialized Once" << std::endl;
    value = "HELLO WORLD";
  });
  return value;
}

void thread_func() {
    auto res = get_initialized_value();
    std::cout << "Thread is running with value: " <<  res << "\n" << std::endl;
}

int main() {
    std::thread t1(thread_func);
    std::thread t2(thread_func);
    std::thread t3(thread_func);

    t1.join();
    t2.join();
    t3.join();

    return 0;
}
