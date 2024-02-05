# 开始一个线程  
方法1：  
`std::thread t1(f1, n + 2);`使用的是定义语句。它在 t1 未被定义的情况下定义一个新的 std::thread 对象 t1，并启动新的线程。  
方法2：  
`t1 = std::thread(f1, n + 2);`使用的是赋值语句。它假设 t1 已经被定义过，然后创建一个新的线程，并将新线程的标识符赋值给 t1。需要注意的是，如果 t1 在赋值之前已经关联了其他的线程（即 t1 是活动的），那么在赋值之前应当对 t1 调用 join() 或 detach()，否则在赋值时 std::thread 的析构函数会被调用，程序会抛出 std::system_error 异常。  
方法3：  
`T1 = std::move(t1);` 使用移动语义，这样的话t1就不再是一个线程。  

```
#include <iostream>
#include <thread>
#include <chrono>

int main(){
  std::thread t1;
  std::thread t2([&](){
      for (int i = 0; i < 10; i++) {
       std::cout << "i am here: " << i << std::endl;
       std::this_thread::sleep_for(std::chrono::seconds(2));
      }
  });
  std::this_thread::sleep_for(std::chrono::seconds(2));
  t1 = std::move(t2);
  if(t2.joinable()){
   std::cout << "t2 joinable" << std::endl;//不会打印
   t2.join();
  }
  if(t1.joinable()){
    std::cout << "t1 joinable" << std::endl;
    t1.join();
  }
  return 0;
}
```
# Thread 相关练习
题目1 ： std::thread  std::vector 结合使用[题目1](https://github.com/NJAUHB/DailyStudy/blob/master/2023/src/Thread/ThreadWithVector.cpp)  
题目2 ：子线程循环 10 次，接着主线程循环 100 次，接着又回到子线程循环 10 次，接着再回到主线程又循环 100 次，如此循环50次[题目2](https://github.com/NJAUHB/DailyStudy/blob/master/2023/src/Thread/ThreadAndSafe.cpp)
