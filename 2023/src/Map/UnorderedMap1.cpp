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
  bool operator==(const Key& other) const { return std::strncmp(a, other.a, sizeof(a)) == 0 && b == other.b; }

  // 重载<运算符用于map
  bool operator<(const Key& other) const {
    int cmp = std::strncmp(a, other.a, sizeof(a));
    if (cmp != 0)
      return cmp < 0;
    return b < other.b;
  }
};

// 为Key结构体定义自定义哈希函数
struct KeyHash {
  std::size_t operator()(const Key& k) const {
    std::size_t h1 = std::hash<std::string>()(std::string(k.a, sizeof(k.a)));
    std::size_t h2 = std::hash<std::string>()(k.b);
    return h1 ^ (h2 << 1);  // 或使用其他合适的哈希组合方法
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
/*
在代码中同时定义了 `MyKeyEqual` 和 `operator==` 时，**优先级取决于上下文使用场景**。以下是具体规则和影响：

---

### **1. 容器中使用 `MyKeyEqual`**
如果 `MyKeyEqual` 被显式指定为容器（例如 `std::unordered_map` 或 `std::unordered_set`）的比较器，容器只会使用 `MyKeyEqual`，而忽略 `operator==`。

```cpp
struct MyKey {
    int id;
    std::string name;

    bool operator==(const MyKey& other) const {
        return id == other.id && name == other.name; // 通用相等规则
    }
};

struct MyKeyEqual {
    bool operator()(const MyKey& lhs, const MyKey& rhs) const {
        return lhs.id == rhs.id; // 特定场景仅比较 id
    }
};

std::unordered_map<MyKey, int, MyKeyHash, MyKeyEqual> my_map; // 使用 MyKeyEqual 进行比较
```

在上述代码中，`std::unordered_map` 会完全依赖 `MyKeyEqual` 的逻辑，与 `operator==` 无关。

---

### **2. 容器中未指定 `MyKeyEqual`**
如果没有显式指定比较器，哈希容器会依赖默认的 `operator==`，前提是 `operator==` 已定义。

```cpp
std::unordered_map<MyKey, int, MyKeyHash> my_map; // 使用 operator== 进行比较
```

在这种情况下，容器使用 `operator==` 来判断键的相等性。

---

### **3. 直接调用 `operator==`**
在代码中直接比较两个对象（如 `a == b`），总是使用 `operator==`，与 `MyKeyEqual` 无关。

```cpp
MyKey a{1, "Alice"};
MyKey b{1, "Bob"};
if (a == b) {  // 使用 operator==
    std::cout << "Keys are equal!" << std::endl;
}
```

即使定义了 `MyKeyEqual`，上述代码仍然只会使用 `operator==`。

---

### **4. 两者的关系与作用范围**

| 特性                         | `MyKeyEqual`                              | `operator==`                      |
|------------------------------|-------------------------------------------|-----------------------------------|
| **使用范围**                 | 仅适用于容器中显式指定的比较逻辑           | 适用于所有直接使用 `==` 的场景    |
| **是否依赖容器**             | 是，通常与哈希容器的模板参数绑定            | 否，独立于容器                   |
| **优先级**                   | 容器中优先级高（显式指定时覆盖 `operator==`）| 通用逻辑优先（在无自定义比较器时）|

---

### **如何选择和避免冲突？**

#### **避免不一致逻辑**
当既有 `MyKeyEqual` 又有 `operator==` 时，确保两者逻辑一致或目的明确。如果有冲突，可能导致意想不到的行为。

#### **推荐使用模式**
- **优先定义 `operator==`**：如果相等比较逻辑是类型本身的通用规则，直接实现 `operator==`。
- **使用 `MyKeyEqual` 定制场景逻辑**：仅当某些容器需要不同的比较逻辑时，定义 `MyKeyEqual`。

例如：

```cpp
struct MyKeyEqual {
    bool operator()(const MyKey& lhs, const MyKey& rhs) const {
        return lhs == rhs; // 直接复用 operator==，避免逻辑重复
    }
};
```

这样可以复用 `operator==` 的逻辑，同时在特定场景中允许扩展。

---

### 总结

- 容器中 **优先使用显式指定的 `MyKeyEqual`**。
- 未指定时，默认使用 `operator==`。
- 直接比较时，总是用 `operator==`。
- 避免在 `MyKeyEqual` 和 `operator==` 中定义冲突的逻辑，否则可能导致难以维护和调试的问题。
*/