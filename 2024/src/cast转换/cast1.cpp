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
    std::shared_ptr<Base> base_ptr = std::make_shared<Derived>();
    std::shared_ptr<Derived> derived_ptr = std::dynamic_pointer_cast<Derived>(base_ptr);

    if (derived_ptr) {
        derived_ptr->print();
    } else {
        std::cout << "Failed to cast base_ptr to derived_ptr" << std::endl;
    }

    return 0;
}