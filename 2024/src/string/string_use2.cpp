#include <iostream>
#include <cstring>

int main() {
    char str1[] = "hello";
    char str2[] = "hello1";
    char str3[] = "hello1";

    if (strcmp(str1, str2) == 0) {
        std::cout << "str1 和 str2 相等" << std::endl;
    } else {
        std::cout << "str1 和 str2 不相等" << std::endl;
    }

    if (strncmp(str1, str3, 5) == 0) {
        std::cout << "str1 和 str3 相等" << std::endl;
    } else {
        std::cout << "str1 和 str3 不相等" << std::endl;
    }

    return 0;
}