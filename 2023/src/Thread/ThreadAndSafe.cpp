#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

const int count = 50;
int flag = 10;
std::condition_variable cv;
std::mutex mtex;

void fun(const int num, const std::string &str) {
    for(int i = 0; i < count; ++i) {
        std::unique_lock<std::mutex> lk(mtex);
        cv.wait(lk, [&]{
            return num == flag;
        });
        for(int j = 0; j < num; ++j) {
            std::cout << str << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
        flag = (flag == 10 ? 100 : 10);
        cv.notify_one();
    }
}

int main(void) {
    auto start = std::chrono::high_resolution_clock::now();
    std::thread child(fun, 10, "child");// child thread
    fun(100, "father");// father thread
    child.join();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    std::cout << elapsed.count() << std::endl;

    return 0;
}