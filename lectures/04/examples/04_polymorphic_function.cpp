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
// This client function only needs the base interface.
void make_speak(const Animal& animal) {
    animal.speak();
}
int main() {
    Animal animal;
    Dog dog;
    make_speak(animal); // Base argument: Animal::speak.
    make_speak(dog); // Public inheritance permits binding to const Animal&.
    // The virtual call in make_speak selects Dog::speak for the Dog object.
    return 0;
}
