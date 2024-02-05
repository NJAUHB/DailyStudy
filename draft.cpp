#include <iostream>

typedef void (*FP)(char* s);
void Invoke(char* s);

int main(int argc,char* argv[])
{
    FP fp;      //通常是用宏FP来声明一个函数指针fp
    fp=&Invoke;
    char a[] = "Hello World!\n";
    fp(a);
    (*fp)(a);
    return 0;
}

void Invoke(char* s)
{
    std::cout << s << std::endl;
}