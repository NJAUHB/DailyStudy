# Posix消息队列#include <mqueue.h>
参考代码Mq1.cpp 编译时需要链接lrt库  `g++ Mq1.cpp -o mq1 -g -lrt`  
___
GCC中常用手动链接选项，我目前用到最多的两个库是-lrt、-lpthread。它们链接都是什么库呢？  

-lrt：手动链接实时库librt。这个库包括异步I/O，消息队列，进程调度，实时信号扩展，信号量，共享内存对象，同步I/O和定时器选项下定义的接口。 

-lpthread：手动链接多线程库pthread，用到多线程需要链接，包括在linux上使用std::thread也需要链接。libpthread库是Linux系统中的线程库文件，它提供了各种线程相关的API函数。使用这个库可以在Linux系统中创建多线程程序，实现程序并发执行，提高程序运行效率。  
___

