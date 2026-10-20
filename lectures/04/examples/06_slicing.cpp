#include <iostream>

class Animal {
public:
    virtual ~Animal() = default;
    virtual void speak() const { std::cout << "Animal sound\n"; }
};
class Dog : public Animal {
public:
    void speak() const override { std::cout << "Woof\n"; }
};

void by_reference(const Animal& animal) { animal.speak(); }
void by_value(Animal animal) { animal.speak(); } // Deliberately demonstrates slicing.

int main() {
    Dog dog;
    std::cout << "reference: ";
    by_reference(dog);
    std::cout << "value: ";
    by_value(dog);
    std::cout << "original: ";
    dog.speak(); // The original Dog has not changed.
    return 0;
}
