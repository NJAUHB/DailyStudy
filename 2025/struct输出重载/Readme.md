
## 通过类型转换符输出
 `UserID` 结构体里有如下定义：

```cpp
operator uint64_t() const { return index; }
```
也就是常说的**类型转换运算符**，允许对象被**隐式转换为 `uint64_t`**。

---

### C++类型转换详细解释

- 当你写 `ostream << id` 时，`operator<<` 期望参数是内置类型（`int, long, uint64_t`等）或者某些库类型。
- `UserID` 没有 `operator<<(std::ostream&, UserID)`，但是有 `operator uint64_t()`。
- **C++ 标准（§13.3.3.1.2）规定**，如果目标类型支持，并且有到该类型的显式或隐式转换（如你的 `operator uint64_t()`），那么会优先用类型转换，再去找流的合适重载：

```cpp
std::ostream& operator<<(std::ostream&, unsigned long long);
```
- `uint64_t` 通常就是 `unsigned long long`，所以
  - 会隐式把 id 转成 `uint64_t`
  - 然后调用标准库的 `operator<<` 实现

---

### 实践举例说明

```cpp
struct UserID {
    UserID(i):index(i){};
    operator uint64_t() const { return index; }
    // ...
};

UserID id(12345);
std::cout << id << std::endl; // 自动等价于 std::cout << static_cast<uint64_t>(id)
```

---

## 如果**没写类型转换函数**则不行？

- 如果**没写类型转换函数**，比如：
  ```cpp
  struct Foo {};
  std::cout << Foo();
  ```
  肯定报错：没有 `operator<<`。

- 但你的 UserID 特别声明了类型转换，**所以能隐式转而成功输出**。

---

## 总结一句话

> 你能直接 `<< id`，是因为 `UserID` 里定义了 `operator uint64_t() const`，C++ 自动把 UserID 隐式转换成整数，然后调用整数的输出流重载。

---