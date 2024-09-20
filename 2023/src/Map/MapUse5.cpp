#include <map>
#include <algorithm>
#include <iostream>

int main() {
    std::map<int, std::string> myMap = {{1, "one"}, {2, "two"}, {3, "three"}};

    // 使用 std::find_if 查找第一个值为 "two" 的键值对
    auto it = std::find_if(myMap.begin(), myMap.end(), [](const std::pair<int, std::string>& elem) {
        return elem.second == "two";
    });

    if (it != myMap.end()) {
        std::cout << "Found: " << it->first << " -> " << it->second << std::endl;
    } else {
        std::cout << "Not found." << std::endl;
    }

    return 0;
}