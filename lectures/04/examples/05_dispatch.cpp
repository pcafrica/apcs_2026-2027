#include <iostream>

class Animal {
public:
    virtual ~Animal() = default;
    void label() const { std::cout << "Animal label\n"; }
    virtual void speak() const { std::cout << "Animal sound\n"; }
};
class Dog : public Animal {
public:
    void label() const { std::cout << "Dog label\n"; }
    void speak() const override { std::cout << "Woof\n"; }
};
int main() {
    Dog dog;
    const Animal& animal = dog;
    dog.label();
    animal.label(); // Non-virtual: the Animal interface selects Animal::label.
    animal.speak(); // Virtual: the object is a Dog, so Dog::speak runs.
    dog.Animal::speak(); // Qualification requests Animal::speak directly.
    return 0;
}
