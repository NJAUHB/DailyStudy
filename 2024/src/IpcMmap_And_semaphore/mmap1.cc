#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#define GiB *(1024LL * 1024 * 1024)

int main() {
    void *p = mmap(NULL, 3 GiB, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (p == MAP_FAILED) {
        std::cout << "mmap failed!!!" << std::endl;
    } else {
        std::cout << "mmap success!!!" << std::endl;
    }

    *(int *)((u_int8_t*)p + 1 GiB) = 100; // 在共享内存1Gib的地方进行赋值
    *(int *)((u_int8_t*)p + 2 GiB) = 200; // 在共享内存2Gib的地方进行赋值
    std::cout << "data = " << *(int *)((u_int8_t*)p + 2 GiB) << std::endl;
    return 0;
}