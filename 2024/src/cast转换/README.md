
# dynamic_pointer_cast 和 static_pointer_cast
是 C++ 中用于智能指针类型转换的两种方法，它们的主要区别在于类型转换的安全性和目的。

## dynamic_pointer_cast
用途：用于将基类的智能指针安全地转换为派生类的智能指针。  

转换类型：运行时类型识别（RTTI）机制下的安全向下转换。  

安全性：在转换过程中，会检查实际对象类型是否与目标类型匹配。如果匹配，返回转换后的指针；如果不匹配，返回 nullptr。  

使用场景：主要用于继承层次中基类指针转换为派生类指针的情况。

示例代码：
cast1.cpp

## static_pointer_cast

用途：用于在已知类型安全的情况下进行智能指针的静态类型转换。  

转换类型：编译时的静态转换。  

安全性：不会进行运行时检查，因此转换必须确保是安全的。错误的转换会导致未定义行为。  

使用场景：主要用于已知类型安全的情况下，例如将派生类指针转换为基类指针，或在无需运行时检查的已知安全转换。  

示例代码：  
cast2.cpp


## 区别总结

类型转换安全性： 
dynamic_pointer_cast：运行时类型检查，安全向下转换。  
static_pointer_cast：编译时类型检查，无运行时开销，适用于已知安全的转换。  

性能：
dynamic_pointer_cast：由于运行时检查，性能稍有影响。
static_pointer_cast：仅在编译时进行检查，无运行时开销，性能更高。

使用场景：
dynamic_pointer_cast：需要向下转换，并且类型不确定时使用。
static_pointer_cast：已知类型安全的转换，无需运行时检查时使用。

## 结论
  选择使用 dynamic_pointer_cast 或 static_pointer_cast 取决于具体的需求和安全性考虑。如果需要进行安全的向下转换并且类型不确定，应该使用 dynamic_pointer_cast。如果类型已知且转换安全，可以使用 static_pointer_cast 以提高性能