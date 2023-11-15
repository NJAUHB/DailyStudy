#include <string>
#include <cstring>
#include <iostream>
int main(){
 std::string str = "Hello,World!";
 char array1[20];
 char array2[20];
 size_t len1 = str.length();
 size_t len2 = str.size();
 if (len1 == len2) {
    std::cout << "len1 == len2, 都为： " << len1  << std::endl;
 }
 strcpy(array1, str.c_str());
 memset(array2, 0, 20);
 std::cout << array2 << std::endl;
 strncpy(array2, str.c_str(), 20);
 std::cout << array2 << std::endl;
 return 0;
}