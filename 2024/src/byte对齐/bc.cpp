#include <iostream>
#include <type_traits>

// 在64位的系统上面
//在64位系统上：
// bool类型通常是1字节。
// int类型通常是4字节。
// short类型通常是2字节。
// double类型通常是8字节。
typedef struct s1
{
    int A; // 4
    char B; // 1
    short C; // 2
}S1;
/*A 占用前4字节，是一个int。
B 紧随其后占用第5字节，是一个char。
紧接着是1字节的填充（padding），以确保c的对齐。
C 占用第7和8字节，是一个short。
*/

typedef struct s2
{
    char Q; // 1
    int W; // 4
    short E; // 2
}S2;
/*
0       1       4       3       8       10      12
+-------+-------+-------+-------+-------+-------+
|   Q   | pad[3]|        W      |   E   | pad[2]|
+-------+-------+-------+-------+-------+-------+
*/

typedef struct s3
{
    bool a; // 1
    S1 ss1; // 8
    double b; // 8
    int c; // 4
}S3;
/*
0       1       4               12        16         24      28      32
+-------+-------+---------------+---------+-----------+-------+-------+
|   a   | pad[3]|       ss1     |  pad[4] |      b    |   c   | pad[4]|
+-------+-------+---------------+---------+-----------+-------+-------+
*/

typedef struct s4
{
    bool a; // 1
    S2 ss2; // 12
    double b; // 8
    int c; // 4
}S4;
/*
0       1       4                   16        24      28      32
+-------+-------+-------------------+---------+-------+-------+
|   a   | pad[3]|        ss2        |    b    |   c   | pad[4]|
+-------+-------+-------------------+---------+-------+-------+
*/

int main() {
    std::cout << "sizeof (S1) = " << sizeof(S1) << std::endl;
    std::cout << "sizeof (S2) = " << sizeof(S2) << std::endl;
    std::cout << "sizeof (S3) = " << sizeof(S3) << std::endl;
    std::cout << "sizeof (S4) = " << sizeof(S4) << std::endl;

    std::cout << "Alignment of S1: " << alignof(S1) << std::endl;
    std::cout << "Alignment of S2: " << alignof(S2) << std::endl;
    std::cout << "Alignment of S3: " << alignof(S3) << std::endl;
    std::cout << "Alignment of S4: " << alignof(S4) << std::endl;
    return 0;
}