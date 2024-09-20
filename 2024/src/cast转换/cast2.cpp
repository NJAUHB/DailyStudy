#include <iostream>
#include <memory>

class Base {
public:
    virtual ~Base() = default;
};

class Derived : public Base {
public:
    void print() { std::cout << "Derived::print()" << std::endl; }
};

int main() {
    std::shared_ptr<Derived> derived_ptr = std::make_shared<Derived>();
    std::shared_ptr<Base> base_ptr = std::static_pointer_cast<Base>(derived_ptr);

    // Converting back using static_pointer_cast
    std::shared_ptr<Derived> derived_ptr_again = std::static_pointer_cast<Derived>(base_ptr);
    derived_ptr_again->print();

    return 0;
}