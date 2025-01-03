#include <iostream>
#include <thread>
#include <mutex>
#include <string>

int get_value() {
    std::cout <<  "run get value!" << std::endl; //执行一次
    return 1;
}

const int& get_initialized_value() {
  /*静态局部变量只在第一次调用时初始化，并且在所有后续调用中保持其值。*/
  static int value = get_value();
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
