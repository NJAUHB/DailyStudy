# 回调函数
## 定义
- **可执行代码**(函数A)作为**参数**(fn)传入**其他的可执行代码**(函数B) 
- 并由其他的可执行代码在T时刻执行这段可执行代码，得到结果P   

生活中的回调函数：  
- 场景一    
有一个房间里没有灯，只有一个 灯座  
你给 灯座 装上了一盏 白炽灯  
到了晚上，你 打开了灯座上的开关  
灯座通了电，点亮了 灯座上的那盏灯  
于是，那盏 白炽灯 发出的 温暖的光 浸满了整个房间  
这里对应了定义描述的五个要素：  
👇  
白炽灯 相当于是 函数 A  
灯座 相当于是 函数 B  
对于灯座来说，灯座上的那盏灯 相当于是 参数 fn  
对于灯座来说，打开灯座上的开关 相当于是 调用的时刻 T  
温暖的光 相当于是 回调结果 P  
- 场景二  
一天，你在房间里开了一个聚会，希望把灯光换成彩色的  
于是，把灯座上的 白炽灯 换成了 彩灯  
当你 打开灯座上的开关时，房间里充满了 彩色的光  
场景二和场景一相比，  
👇
相同的要素是：灯座、灯座上的那盏灯、打开灯座上的开关  
不同的要素是：彩灯、彩色的光  

这里说明了**回调函数的设计初衷** —— 利用 依赖注入 (dependency injection, DI) 实现 控制反转 (Inversion of Control, IoC) 和 依赖倒置 (dependency inversion principle, DIP)，即**将控制权下发给回调函数**：  
灯座 不需要知道灯座上的灯是 白炽灯 还是 彩灯  
灯座 只需要知道 灯座上有一盏灯  
灯座 在你 打开灯座上的开关时，会点亮 灯座上有一盏灯  
这样就可以 换上不同的灯泡，得到不同颜色的灯光。  

好的，下面详细讲解C++中的回调函数、函数指针与同步/异步回调。

---

**回调函数（Callback Function）**本质上是“把函数作为参数传递给另一个函数，在适当时机被调用”。  
—— 是一种常用的**解耦**、**事件驱动**、**高阶函数**手段。

在C/C++中主要有 3 种实现方式：

- 传统C风格“函数指针”；
- 用类成员函数+对象指针（或静态成员函数）；
- （C++11及以后）使用`std::function`与`lambda`表达式。

---
 
简而言之，回调函数就是**允许用户把需要调用的方法(函数A)的指针作为参数(fn)**传递给一个**函数(函数B)**，以便该函数(函数B)在处理相似事件的时候可以灵活的使用不同的方法。
```
int Callback()    // /< 回调函数
{
    // TODO
    return 0;
}
int main()     // /<  主函数
{
    // TODO
    Library(Callback);  // /< 库函数通过函数指针进行回调
    // TODO
    return 0;
}
```
# 同步 和 异步 回调
## 深入讨论  
实际上，不管是什么程序设计语言，回调函数都分为 同步 和 异步 两种。 

在同步回调中，回调函数在主函数内部就地执行，主函数会等待回调函数执行完毕后再继续执行。  
- 同步方式：  
通过 参数 传递回调函数  
调用者 立即调用 回调函数（调用时刻 在函数返回前）  
此处的 回调函数 和 调用者 调用栈相同  

在异步回调中，主函数和回调函数在不同的线程中执行，主函数不会等待回调函数执行完毕。  
- 异步方式：
通过 注册（例如 signal 函数）设置回调函数  
调用者 先存储 回调函数，在未来的某个 调用时刻，取出并调用 回调函数  
此处的 回调函数 和 调用者 调用栈不同  

当然可以！下面我将分别讲解**函数指针**、`std::function`以及**Lambda函数**，重点说明各自的概念、用法、示例、区别和适用场景。

----
☚☚☚☚☚☚☛☛☛☛☛☛
----

# 一、函数指针

### 1. 概念

函数指针就是一个指向函数的指针变量。通过它可以间接调用指定的函数。

**适用场景：**  
- C风格API的回调，比如`qsort`自定义比较；  
- 想传递或存储函数时，但对类型和签名有限制。

### 2. 基本语法

```cpp
返回值类型 (*指针变量名)(参数类型列表)
```

### 3. 示例

#### 1. 定义和使用函数指针

```cpp
#include <iostream>
using namespace std;

void hello(int x) {
    cout << "Hello, x = " << x << endl;
}

int main() {
    void (*funcPtr)(int) = hello; // 定义一个函数指针并初始化
    funcPtr(42);                  // 通过函数指针调用函数
}
```
输出：
```
Hello, x = 42
```
| 写法                              | 合法性 | 说明                 |
| ------------------------------- | --- | ------------------ |
| `void (*fun_p)() = say_hello;`  | ✅   | 推荐写法               |
| `void (*fun_p)() = &say_hello;` | ✅   | 明确写法，也完全合法         |
| `fun_p();`                      | ✅   | 调用函数               |
| `(*fun_p)();`                   | ✅   | 解引用调用，等效于上面        |


#### 2. 传递函数指针作为参数

```cpp
void callFunc(void (*f)(int), int n) {
    f(n);
}

int main() {
    callFunc(hello, 99);
}
```

#### 3. 使用typedef或using简化

```cpp
#include <iostream>

// 原始函数
void say_hello(int x) {
    std::cout << "Hello, value: " << x << std::endl;
}

// typedef 写法
typedef void (*HelloFuncTypedef)(int);

// using 写法（推荐）
using HelloFuncUsing = void (*)(int);

int main() {
    // 使用 typedef 类型
    HelloFuncTypedef func1 = say_hello;
    func1(1);

    // 使用 using 类型
    HelloFuncUsing func2 = say_hello;
    func2(2);

    return 0;
}

```

#### 4. 注意事项

- 函数指针**只能指向函数名或者有相同比例签名的全局或静态成员函数**。
- 不能直接指向非静态成员函数（有this指针）。

---

# 二、std::function

### 1. 概念

`std::function`是C++11引入的**通用可调用对象包装类**，可包装各种可调用类型：普通函数、函数指针、Lambda表达式、仿函数和成员函数等。

**适用场景：**  
- 支持更复杂回调、延迟绑定；  
- 可以存储、传递**任意可调用对象**，如lambda、bind表达式等。

### 2. 基本语法

```cpp
#include <functional>

std::function<返回类型(参数列表)> func;
```

### 3. 示例

#### 1. 简单用法

```cpp
#include <iostream>
#include <functional>
using namespace std;

void hello(int x) { cout << "hello: " << x << endl; }

int main() {
    std::function<void(int)> f = hello; // 包装函数
    f(7);
}
```

#### 2. 接受lambda/仿函数

```cpp
std::function<int(int, int)> calc = [](int a, int b){ return a + b; };
cout << calc(10, 20) << endl; // 输出30
```

#### 3. 作为参数类型用

```cpp
void process(std::function<void()> cb) {
    cb();
}
process([](){ cout << "callback" << endl; });
```

#### 4. 支持成员函数+对象

```cpp
struct A { void run(int n) { cout << "A::run: " << n << endl; } };

int main() {
    A obj;
    std::function<void(int)> f = [&obj](int n){ obj.run(n); };
    f(555);
}
```

#### 5. 结合std::bind

```cpp
#include <functional>
#include <iostream>
void foo(int a, int b) { std::cout << a << "," << b << std::endl; }
int main() {
    auto bound = std::bind(foo, 1, std::placeholders::_1); // 只绑定第一个参数
    bound(99); // 实际调用foo(1, 99)
}

// 使用 std::bind 创建一个绑定了第一个参数的函数对象：

// foo 是目标函数。

// 1 是传给 foo 的第一个参数（提前绑定）。

// std::placeholders::_1 是一个占位符，表示这个位置将在调用 bound(...) 时由调用者传入。

// std::bind 返回的是一个可调用对象（不是函数指针）。

// 对于复杂函数推荐使用 std::function 包裹：

// std::function<void(int)> bound = std::bind(foo, 1, std::placeholders::_1);
```

#### 6. 注意事项

- `std::function`内部有类型擦除，较函数指针略慢（但更灵活）。
- 可以为`std::function`赋值为空（可检测是否有效）。

---

# 三、Lambda函数（匿名函数）

### 1. 概念

**Lambda表达式**是C++11引入的一种匿名函数（可捕获变量），提供简洁的写法，可以在需要函数对象的地方直接定义和使用。

**适用场景：**  
- 内联、短小、无需重用的回调和算法；  
- 易于在局部作用域捕获变量并闭包。

### 2. 基本语法

```cpp
[capture](parameter_list) -> return_type { function_body }
```
- `[]`：捕获列表，用于捕获外部变量（可省略）。
- `(parameter_list)`：参数列表。
- `-> return_type` ：返回类型（可省略，C++14起可自动推断）。
- `{}`：函数体。  

| 捕获方式               | 示例                     | 说明                           |
| ------------------ | ---------------------- | ---------------------------- |
| **值捕获（拷贝）**        | `[x]`                  | 拷贝 `x` 的值到 lambda 内部         |
| **引用捕获**           | `[&x]`                 | 捕获 `x` 的引用，可在 lambda 内修改外部变量 |
| **隐式值捕获**          | `[=]`                  | 所有使用到的外部变量都按值捕获              |
| **隐式引用捕获**         | `[&]`                  | 所有使用到的外部变量都按引用捕获             |
| **混合捕获**           | `[=, &y]`              | 默认值捕获，但 `y` 按引用捕获            |
| **初始化捕获（C++14 起）** | `[val = x + 1]`        | 类似构造函数方式捕获变量，可重命名            |
| **移动捕获（C++14 起）**  | `[ptr = std::move(p)]` | 捕获右值并转移所有权（适用于智能指针等）         |
```cpp

// 初始化捕获（C++14+）
auto f6 = [z = x + y]() { std::cout << z << std::endl; };

// 移动捕获（C++14+）
auto p = std::make_unique<int>(42);
auto f7 = [ptr = std::move(p)]() { std::cout << *ptr << std::endl; };

```

### 3. 示例

#### 1. 最简用法

```cpp
auto add = [](int a, int b) { return a + b; };
std::cout << add(1, 2) << std::endl;
```

#### 2. 捕获外部变量

```cpp
int x = 10, y = 100;
auto f = [x, &y](int z) { return x + y + z; }; // x值捕获，y引用捕获
y = 200;
std::cout << f(5) << std::endl; // 10 + 200 + 5 = 215
```

#### 3. 用作回调参数

```cpp
#include <vector>
#include <algorithm>
std::vector<int> v = {1, 4, 5};
std::for_each(v.begin(), v.end(), [](int n){ std::cout << n << " "; });
```

#### 4. 用于std::function

```cpp
std::function<void()> cb = [] { std::cout << "Lambda as callback\n"; };
cb();
```

#### 5. 可变捕获

```cpp
int cnt = 0;
auto lam = [cnt]() mutable { cnt += 1; std::cout << cnt << std::endl; };
lam(); // 输出1
lam(); // 输出2
```

#### 6. 带返回值类型（C++11）

```cpp
auto max = [](int a, int b) -> int { return a > b ? a : b; };
```

#### 7. 可以存储于变量、传参数、立即执行
```cpp
[](const std::string& s){ std::cout << s << std::endl; }("hello world");
```

---

# 四、三者对比与选择建议

|方式           | 可否捕获变量 | 可否指向成员 | 类型推断 | 灵活性 | 典型场景         |
|---------------|-------------|-------------|----------|--------|----------------|
|函数指针       | 不能        | 静态成员    | 否       | 低     | C风格接口、极致性能、老代码|
|std::function  | 支持        | 支持        | 部分     | 高     | 灵活回调、存储任意可调用类型、现代C++库|
|Lambda         | 支持        | 可封装成员  | 是       | 极高   | 局部回调、算法、现代事件处理|

- **函数指针**：语法古老简单，适合C接口和要求极致性能、无类型擦除的情形。
- **std::function**：支持所有类型回调和闭包，灵活且易于和STL/自定义模板代码交互。
- **Lambda表达式**：适合直接写便捷、可捕获外部变量的本地函数，也可作为参数或赋值给std::function。

---

# 五、综合示例

```cpp
#include <iostream>
#include <functional>

// 普通函数
void f1(int x) { std::cout << "f1: " << x << std::endl; }

// 仿函数
struct F2 { void operator()(int x) const { std::cout << "F2: " << x << std::endl; } };

int main() {
    // 1. 函数指针
    void (*p)(int) = f1;
    p(1);

    // 2. std::function
    std::function<void(int)> ff = f1;
    ff(2);
    ff = F2();
    ff(3);

    // 3. Lambda
    ff = [](int a){ std::cout << "lambda: " << a << std::endl; };
    ff(4);

    // Lambda直接用
    auto lam = [](int a) { return a+10; };
    std::cout << "lam(5): " << lam(5) << std::endl;
}
```

输出：
```
f1: 1
f1: 2
F2: 3
lambda: 4
lam(5): 15
```

---

# 六、推荐阅读与总结

- 现代C++编程中，如非性能极限或API约束，推荐用**lambda + std::function**。
- 仅必要场合（如低级接口/跨C代码）用**函数指针**。
- Lambda表达式通常写一次用一次，不用命名，也可以作为`std::function`的对象。

如需了解更高级的用法（如成员函数指针、捕获this、泛型lambda、可变参数lambda），欢迎进一步提问！