# 字符串长度
对于C++ `std::string`对象，可以使用成员函数length()或size()来获得字符串的长度，这两个函数返回相同的值。
参考代码string_use1.cpp
# 字符串复制到char数组
strcpy和strncpy都是**C语言标准库**中用于复制字符串的函数，不过这两个函数在行为上有一些重要区别，尤其是在处理字符串长度和终止符时。  

`strcpy(destination, source)`; strcpy函数将source字符串复制到destination数组中，并自动在复制的字符串末尾添加空字符（\0）。**使用strcpy时必须确保destination数组足够大，可以包含source字符串及其空终止符，否则可能会导致缓冲区溢出**。
>因为strcpy这个函数不检查目标缓冲区的长度，所以如果source字符串的长度超出了destination数>组的大小，就>会发生越界写入，这是造成安全隐患的常见原因。  

`strncpy(destination, source, num)`; strncpy函数复制至多num个字符从source字符串到destination数组。如果source的长度小于num，则剩余的部分会被空字符填充。**这个函数通常用于防止缓冲区溢出，因为它允许指定一个最大的复制长度。**

但是，strncpy有一个非常重要的特性：**如果source字符串的长度大于或等于num，则复制的结果不会被空字符终止。如果发生这种情况，那么destination数组将不包含终止空字符，这可能导致例如printf等需要字符串空终止的函数行为异常。**  

在提供的例子中：
```
char entity_version[256];
strcpy(entity_version, "C++ is cool");
```
这行代码会安全地工作，并自动在复制的字符串末尾添加空字符。
```
strncpy(entity_version, "C++ is cool", 255);
```
这行代码将复制最多255个字符到entity_version数组中。由于"C++ is cool"字符串的长度小于255，因此剩余的字符将被填充为空字符。

# 字符串相等判断  
[详见](https://juejin.cn/s/c%2B%2B%E6%80%8E%E4%B9%88%E5%88%A4%E6%96%AD%E5%AD%97%E7%AC%A6%E4%B8%B2%E6%98%AF%E5%90%A6%E7%9B%B8%E7%AD%89)


