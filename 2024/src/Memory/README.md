# C++ 内存管理
## 计算机内存结构

Stack（栈）

- 从高地址向低地址增长。由编译器自动管理分配。程序中的局部变量、函数参数值、返回变量等存在此区域。  
- 和堆一样存储在计算机 RAM 中。  
- 在栈上创建变量的时候会扩展，并且会自动回收。  
- 相比堆而言在栈上分配要快的多。  
- 用数据结构中的栈实现。  
- 存储局部数据，返回地址，用做参数传递。  
- 当用栈过多时可导致栈溢出（无穷次（大量的）的递归调用，或者大量的内存分配）。  
- 在栈上的数据可以直接访问（不是非要使用指针访问）。  
- 如果你在编译之前精确的知道你需要分配数据的大小并且不是太大的时候，可以使用栈。  
- 当你程序启动时决定栈的容量上限。  
- 生长方向是向下的，是向着内存地址减小的方向增长。

Heap（堆）

- 从低地址向高地址增长。容量大于栈，程序中动态分配的内存在此区域。  
- 和栈一样存储在计算机RAM。
- 在堆上的变量必须要手动释放，不存在作用域的问题。数据可用 delete, delete[] 或者 free 来释放。
- 相比在栈上分配内存要慢。
- 通过程序按需分配。
- 大量的分配和释放可造成内存碎片。
- 在 C++ 中，在堆上创建数的据使用指针访问，用 new 或者 malloc 分配内存。
- 如果申请的缓冲区过大的话，可能申请失败。
- 在运行期间你不知道会需要多大的数据或者你需要分配大量的内存的时候，建议你使用堆。
- 可能造成内存泄露。
- 生长方向是向上的，也就是向着内存地址增加的方向.


BSS（Block started by symbol)
  
- 存放未初始化的全局和静态变量。（默认设为0）  

Data Segment (数据区）
  
- 存放已初始化的全局和静态变量， 常量数据（如字符串常量）。

Code Segment（代码区）
  
- 存放可执行程序的机器码。

### 什么是栈和堆？

栈  
是为执行线程留出的内存空间。当函数被调用的时候，栈顶为局部变量和一些 bookkeeping 数据预留块。当函数执行完毕，块就没有用了，可能在下次的函数调用的时候再被使用。栈通常用后进先出（LIFO）的方式预留空间；因此最近的保留块（reserved block）通常最先被释放。这么做可以使跟踪堆栈变的简单；从栈中释放块（free block）只不过是指针的偏移而已。

堆  
是为动态分配预留的内存空间。和栈不一样，从堆上分配和重新分配块没有固定模式；你可以在任何时候分配和释放它。这样使得跟踪哪部分堆已经被分配和被释放变的异常复杂；有许多定制的堆分配策略用来为不同的使用模式下调整堆的性能。  

每一个线程都有一个栈，但是每一个应用程序通常都只有一个堆（尽管为不同类型分配内存使用多个堆的情况也是有的。

### 它们在多大程度上受操作系统或语言运行时的控制？
当线程创建的时候，操作系统（OS）为每一个系统级（system-level）的线程分配栈。通常情况下，操作系统通过调用语言的运行时 （runtime） 去为应用程序分配堆。

### 他们的范围是什么？
栈附属于线程，因此当线程结束时栈被回收。堆通常通过运行时在应用程序启动时被分配，当应用程序（进程）退出时被回收。   

### 它们的大小由什么决定？
当线程被创建的时候，设置栈的大小。在应用程序启动的时候，设置堆的大小，但是可以在需要的时候扩展（分配器向操作系统申请更多的内存）。

### 谁更快
The stack is faster because the access pattern makes it trivial to allocate and deallocate memory from it (a pointer/integer is simply incremented or decremented), while the heap has much more complex bookkeeping involved in an allocation or deallocation. Also, each byte in the stack tends to be reused very frequently which means it tends to be mapped to the processor's cache, making it very fast. Another performance hit for the heap is that the heap, being mostly a global resource, typically has to be multi-threading safe, i.e. each allocation and deallocation needs to be - typically - synchronized with "all" other heap accesses in the program.
