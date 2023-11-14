// CPP Program to demonstrate the use of copy constructor 
#include <iostream> 
#include <stdio.h> 
using namespace std; 
  
class storeVal { 
public: 
    // Constructor 
    storeVal() {} 
    // Copy Constructor 
    storeVal(const storeVal& s) 
    { 
        cout << "Copy constructor has been called " << endl; 
    } 
}; 
  
// Driver code 
int main() 
{ 
    storeVal obj1; // 使用默认构造函数
    storeVal obj2 = obj1;  // 使用拷贝构造函数
    getchar(); 
    return 0; 
}