#include <iostream>

class Engine {
public:
    void start() { std::cout << "Engine started\n"; }
};
class Car {
public:
    Car(Engine& e) : engine(&e) {}
    void drive() { engine->start(); }
private:
    Engine* engine; // Borrowed; supplied through a non-null reference.
};
int main() {
    Engine engine;
    {
        Car car{engine};
        car.drive();
    } // Car is destroyed, but does not delete engine.
    engine.start(); // The independently managed Engine still exists.
    return 0;
}
