# 友元类
友元类(friend class)  
1. 友元类可以访问与之为友元关系的类的所有私有（private）和保护（protected）成员。

2. 非传递性: 友元关系不是传递的。如果类A是类B的友元，类B是类C的友元，这并不意味着类A自动成为类C的友元。

3. 非对称性: 如果类A是类B的友元，这不代表类B也是类A的友元。友元关系是单向的，除非两个类互相声明对方为友元。

4. 声明位置的灵活性: 友元类的声明可以出现在类定义的任何位置，无论是在私有、保护还是公共部分。

5. 不能被继承: 友元关系不能被继承。如果类B是类A的友元，并且C是B的派生类，这并不意味着C也是A的友元。