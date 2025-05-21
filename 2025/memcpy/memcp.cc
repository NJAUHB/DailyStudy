#include <iostream>
#include <cstring> // memcpy

struct Point { int x, y; };

int main() {
    Point p1 = {3, 4};
    Point p2;
    memcpy(&p2, &p1, sizeof(Point));
    std::cout << p2.x << std::endl;
    char buf[10] = "123456789";
memcpy(buf+2, buf, 5); // 未定义行为！
    // p2.x == 3, p2.y == 4
}