# std::call_once
std::call_once 是一个非常有用的工具，用于确保某个**代码段只被执行一次**。这在实现单例模式、懒初始化或者执行只需运行一次的初始化代码时特别有用。std::call_once 使用一个 std::once_flag 标志来记录代码是否已经执行过。  
参考代码co1.cc  


