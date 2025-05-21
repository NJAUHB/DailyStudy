#include <iostream>
#include <cstring>

int main() {
    int a = 0x12345678;
    char bytes[4];
    memcpy(bytes, &a, 4);

    for (int i = 0; i < 4; ++i) {
        printf("第一种打印方式: bytes[%d]=0x%X\n", i, bytes[i]);
        std::cout << "第二种打印方式: bytes[" << i << "] = [0x" << std::hex << (static_cast<unsigned int>(static_cast<unsigned char>(bytes[i]))) << "]" << std::endl;
    }  
}
// bytes[0]=0x78
// bytes[1]=0x56
// bytes[2]=0x34
// bytes[3]=0x12