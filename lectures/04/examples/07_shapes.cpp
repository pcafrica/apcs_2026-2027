#include <iostream>

class Shape {
public:
    virtual ~Shape() { std::cout << "~Shape\n"; }
    virtual double area() const = 0;
    virtual void draw() const = 0;
};
// Still abstract: draw is implemented, but area is not.
class Polygon : public Shape {
public:
    void draw() const override { std::cout << "Drawing a polygon\n"; }
};
class Square : public Polygon {
public:
    Square(double s) : side(s) {}
    double area() const override { return side * side; }
    double get_side() const { return side; }
    ~Square() override { std::cout << "~Square\n"; }
private:
    double side;
};
class Triangle : public Polygon {
public:
    Triangle(double b, double h) : base(b), height(h) {}
    double area() const override { return 0.5 * base * height; }
    ~Triangle() override { std::cout << "~Triangle\n"; }
private:
    double base, height;
};
// Ownership contract: the caller must delete a non-null returned pointer.
Shape* make_shape(int choice) {
    if (choice == 1) return new Square{3.0};
    if (choice == 2) return new Triangle{4.0, 2.0};
    return nullptr;
}
void print_area(const Shape& shape) {
    std::cout << "area=" << shape.area() << '\n';
}
int main() {
    std::cout << "1: square, 2: triangle\n";
    int choice = 0;
    if (!(std::cin >> choice)) {
        std::cerr << "Invalid input\n";
        return 1;
    }
    Shape* shape = make_shape(choice);
    if (shape == nullptr) {
        std::cerr << "Choose 1 or 2\n";
        return 1;
    }
    shape->draw(); // Inherited implementation from the still-abstract Polygon.
    print_area(*shape);
    Square* square = dynamic_cast<Square*>(shape);
    if (square != nullptr)
        std::cout << "side=" << square->get_side() << '\n';
    else
        std::cout << "Not a square\n";
    delete shape; // Virtual destructor; square is a borrowed alias, not an owner.
    // Do not use shape or square after deletion; do not delete square separately.
    return 0;
}
