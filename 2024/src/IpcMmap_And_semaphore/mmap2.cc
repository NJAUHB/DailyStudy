#include <sys/mman.h>
#include <iostream>
#include <fcntl.h> // open
#include <string.h> // strcpy
#include <unistd.h> // close


int main() {
    //把磁盘文件放入共享内存，这样可以使用指针访问磁盘文件test.txt。test.txt必须有内容。
    int fd = open("test.txt", O_RDWR | O_CREAT, 0644);
    if (fd < 0) {
        std::cout << "open error!" << std::endl;
        exit(1);
    }

    //申请内存映射
    void *p = mmap(NULL, 100, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); // MAP_SHARED: 第四个参数指定映射区域的修改对其他进程可见，并且会同步到磁盘文件。
    if (p == MAP_FAILED) {
        std::cout << "mmap failed!!!" << std::endl;
    } else {
        std::cout << "mmap success!!!" << std::endl;
    }
    strcpy((char *)p, "abc"); // (char *)p: 将 p 强制转换为 char * 类型, 以便将字符串 "abc" 复制到映射的内存区域。
    int ret = munmap(p, 100); //释放共享内存
    if (ret == -1) {
       std::cout << "unmap error!" << std::endl;
        exit(1); 
    }
    close(fd);
    return 0;
}