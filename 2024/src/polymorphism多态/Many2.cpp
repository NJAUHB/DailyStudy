#include <iostream>
#include <memory>


// 抽象基类声明
class Animal {
public:
    virtual void makeSound() const = 0;  // 纯虚函数
};

// 不同的子类
class Dog : public Animal {
public:
    void makeSound() const override {
        std::cout << "Woof-woof!" << std::endl;
    }
};

class Cat : public Animal {
public:
    void makeSound() const override {
        std::cout << "Meow-meow!" << std::endl;
    }
};

// 工厂接口
class AnimalFactory {
public:
    virtual std::unique_ptr<Animal> createAnimal() const = 0;  // 工厂方法返回 Animal 基类的智能指针
};

// 不同的具体工厂
class DogFactory : public AnimalFactory {
public:
    std::unique_ptr<Animal> createAnimal() const override {
        return std::make_unique<Dog>();
    }
};

class CatFactory : public AnimalFactory {
public:
    std::unique_ptr<Animal> createAnimal() const override {
        return std::make_unique<Cat>();
    }
};

// 示例
int main() {
    DogFactory dogFactory;
    auto dog = dogFactory.createAnimal();
    dog->makeSound();  // 输出："Woof-woof!"

    CatFactory catFactory;
    auto cat = catFactory.createAnimal();
    cat->makeSound();  // 输出："Meow-meow!"

    return 0;
}