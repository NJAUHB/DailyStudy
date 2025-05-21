#include <iostream>
#include <thread>
#include <vector>
void T1(int a) {
  std::cout << "T1 -> " << a << std::endl;
}
void T2(const int& a) {
  std::cout << "T2 -> " << a << std::endl;
}
int main() {
  std::vector<std::thread> t_vectory;
  std::vector<std::thread> t_vectory_;
  for (int i = 0; i < 3; i++) {
    /* 因为std::vector::push_back函数只接受一个参数，即要添加到向量的元素。
        * 无法直接将函数对象 T1 和整数 i 一起传递给push_back函数。
        * 
        * 即将已经构造好的对象（通过复制构造或移动构造方式）插入到向量的末尾。
        * 它只接受一个参数，即已经构造好的对象本身。
        * 如果要使用push_back向向量中添加线程对象，需要先创建一个线程对象，然后将其推入向量中.
        */
    t_vectory.push_back(std::thread(T1, i));
    /* 先将i赋值给临时变量tmp，
        * 如果直接将i通过引用的方式传递给T2，由于循环变量i在每次迭代过后会被修改，
        * 所以在某些迭代中，T2线程可能会打印该修改后的值，而不是迭代值。
        * 不符合预期
        */
    int tmp = i;
    /* emplace_back 在容器尾部添加一个元素，这个元素原地构造，不需要触发拷贝构造和转移构造。
        * 而且调用形式更加简洁，直接根据参数初始化临时对象的成员。
        * push_back 首先会创建这个元素，然后再将这个元素拷贝或者移动到容器中（
        * 如果是拷贝的话，事后会自行销毁先前创建的这个元素）
        */
    t_vectory_.emplace_back(T2, std::ref(tmp));
  }
  for (auto& thread : t_vectory)
    thread.join();  // 线程必须join() or detach()
  for (auto& thread : t_vectory_)
    thread.join();
  std::cout << "all thread ended " << std::endl;
  return 0;
}