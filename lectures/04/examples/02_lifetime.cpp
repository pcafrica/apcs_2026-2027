#include <iostream>

class Tag {
public:
    Tag() { std::cout << "Tag constructed\n"; }
    ~Tag() { std::cout << "Tag destroyed\n"; }
};
class Animal {
public:
    Animal(int initial_age) : age(initial_age) {
        std::cout << "Animal constructed, age=" << age << '\n';
    }
    ~Animal() { std::cout << "Animal destroyed\n"; }
    int get_age() const { return age; }
private:
    int age;
};
class Dog : public Animal {
public:
    Dog(int age) : Animal(age), tag() {
        std::cout << "Dog constructor body\n";
    }
    Dog() : Dog(1) {} // Delegate to the constructor above.
    ~Dog() { std::cout << "Dog destructor body\n"; }
private:
    Tag tag; // A member object: this dog's identification tag.
};
int main() {
    Dog dog{4}; // Automatic object; no deletion through Animal*.
    std::cout << "Dog age=" << dog.get_age() << '\n';
    return 0;
}
