#include <iostream>
int main() {
  const char* a[] = {
      "hello", "my",
      "friend"}; /* 需要加上const， 否则会提示警告warning: ISO C++ forbids converting a string constant to ‘char*’
                                                  * 字符串常量是不可更改的，当试图将一个字符串字面量（常量）转换成char*（非常量）时，
                                                  * C++编译器会发出警告，因为这可能会让人误以为这个指针指向的字符串是可以修改的。
                                                  */
  std::cout << "sizeof(a) = " << sizeof(a) << std::endl;           // 输出24（3*8）
  std::cout << a[0] << " " << a[1] << " " << a[2] << std::endl;    // 输出的是 char* 类型的指针
  std::cout << &a[0] << " " << &a[1] << " " << &a[2] << std::endl; /* 0x7ffcc10bf0d0 0x7ffcc10bf0d8 0x7ffcc10bf0e0
    输出的是 char* 类型指针的指针
    可以看到每个地址相差8字节，这是由于每个元素是一个指针变量占八个字节
    */
  const char** s = a; /* 为什么能把 a赋给s,因为数组名a代表数组元素内存单元的首地址，即 a = &a[0] = 0x7ffcc10bf0d0;
    而 0x7ffcc10bf0d0即 a[0]中保存的又是字符串"hello"的首地址。
    即 *s = "hello";
    */
  std::cout << **s << " " << **(s + 1) << " " << **(s + 2) << std::endl;  // 输出 h m f
  return 0;
}