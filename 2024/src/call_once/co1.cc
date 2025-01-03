#include <iostream>
#include <thread>
#include <mutex>
#include <string>

std::once_flag flag;
/*
这里的std::once_flag flag作为全局变量存放是为了确保对它的访问在整个程序生命周期内都是唯一的。
放到函数DataInDb内部的话，它将成为局部变量，每次调用函数时都会创建一个新的实例，这样的话std::call_once就无法起到应有的作用。
但是如果将flag作为一个静态变量放在函数内部，那么它将只初始化一次，即在函数第一次调用时。在后续的函数调用中，该静态变量会保持它的值。
因此，将std::once_flag置为静态变量并放在函数内部也能实现std::call_once的目标。
*/
const std::string& get_initialized_value() {
  /*静态局部变量只在第一次调用时初始化，并且在所有后续调用中保持其值。
    所有线程在调用 std::call_once 后都会访问相同的静态局部变量。*/
  static std::string value;
  /*
  std::call_once 是一个线程安全的工具。它接受一个 std::once_flag 和一个可调用对象（在这个例子中是一个 lambda 表达式）。
  当多线程环境中多个线程尝试同时调用 get_initialized_value 时，std::call_once 保证其内部的 lambda 只会执行一次。
  即使多个线程同时到达这一点，只有一个线程会执行 lambda，其他线程会等到这个 lambda 执行完成，然后继续执行其余的代码。
  */
  std::call_once(flag, []() {
    std::cout << "Initialized Once" << std::endl;
    value = "HELLO WORLD";
  });
  return value;
}
/*
std::once_flag flag;

const std::string& get_initialized_value() {
     std::string value; // 局部变量
    std::call_once(flag, [&]() { // 捕获所有外部变量，包括 value
        std::cout << "Initialized Once" << std::endl;
        value = "HELLO WORLD";
    });
    return value; // 返回引用
}
（编译不过）在C++中，局部变量的生命周期是与其创建的作用域绑定的。当控制流离开创建局部变量的作用域，局部变量就会被销毁，其内存被释放。

在你的代码中，局部变量value是在get_initialized_value函数中创建的。所以，当get_initialized_value函数返回时，value的生命周期就结束了，它将被销毁。

然后，你的lambda函数捕获了value的引用，尝试在std::call_once的回调中对其进行初始化。实际上这是一种错误的使用，如果在函数返回后再访问这个已销毁的局部变量，结果将是未定义的，可能会导致一些难以预见的错误。

为了保证value的生命周期比get_initialized_value函数的生命周期要长，应该将value声明为静态局部变量。


----------------------

const std::string get_initialized_value() {
  std::string value;
  std::call_once(flag, []() {
    std::cout << "Initialized Once" << std::endl;
    value = "HELLO WORLD";
  });
  return value;
}
（可以编译）这样修改可以让函数在每次被调用时，都返回一个新的std::string对象。此对象是通过复制（或移动，如果编译器可以优化）局部变量value来创建的。
这个修改让函数在每次调用后返回的都是一个有效的std::string对象，避免了在函数返回后再去访问已经销毁了的局部变量。

但是逻辑有问题，打印
Initialized Once
Thread is running with value: HELLO WORLD

Thread is running with value: 

Thread is running with value:
*/

void thread_func() {
  auto res = get_initialized_value();
  std::cout << "Thread is running with value: " << res << "\n" << std::endl;
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
