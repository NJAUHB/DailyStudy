#include <iostream>
#include <map>
#include <unordered_map>
#include <string>
#include <cstring>

// 定义Key结构体
struct Key {
    char a[10];
    std::string b;

    // 重载==运算符用于unordered_map
    bool operator==(const Key& other) const {
        return std::strncmp(a, other.a, sizeof(a)) == 0 && b == other.b;
    }

    // 重载<运算符用于map
    bool operator<(const Key& other) const {
        int cmp = std::strncmp(a, other.a, sizeof(a));
        if (cmp != 0) return cmp < 0;
        return b < other.b;
    }
};

// 为Key结构体定义自定义哈希函数
struct KeyHash {
    std::size_t operator()(const Key& k) const {
        std::size_t h1 = std::hash<std::string>()(std::string(k.a, sizeof(k.a)));
        std::size_t h2 = std::hash<std::string>()(k.b);
        return h1 ^ (h2 << 1); // 或使用其他合适的哈希组合方法
    }
};

int main() {
    // 使用std::map
    std::map<Key, std::string> map_example;
    Key key1 = {"key1", "value1"};
    Key key2 = {"key2", "value2"};
    map_example[key1] = "This is key1";
    map_example[key1] = "This is key2";

    std::cout << "std::map contents:" << std::endl;
    for (const auto& pair : map_example) {
        std::cout << pair.second << std::endl;
    }

    // 使用std::unordered_map
    std::unordered_map<Key, std::string, KeyHash> unordered_map_example;
    unordered_map_example[key1] = "This is key1";
    unordered_map_example[key2] = "This is key2";

    std::cout << "std::unordered_map contents:" << std::endl;
    for (const auto& pair : unordered_map_example) {
        std::cout << pair.second << std::endl;
    }

    return 0;
}
