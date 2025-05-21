#include <iostream>
int main() {
  int len = 5;
  while (len < 10) {
    int num_array1
        [len]; /*这一行在每次循环时，都会为num_array1创建一个新的局部变量（一个新的数组），这个新数组的寿命仅在这一次循环内。
上，在每次循环中，你都在创建一个新的，大小为len的数组，而旧的数组在每次循环结束时都会被销毁。*/
    std::cout << num_array1 << " sizeof num_array " << sizeof(num_array1) << std::endl;
    len++;
  }
  return 0;
}