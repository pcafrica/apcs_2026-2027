#include <iostream>

class Printer {
public:
    void start() const { std::cout << "Printer started\n"; }
};
class Scanner {
public:
    void start() const { std::cout << "Scanner started\n"; }
};
class Copier : public Printer, public Scanner {
public:
    void copy() const {
        Printer::start();
        Scanner::start();
    }
};
int main() {
    Copier copier;
    copier.copy();
    // copier.start(); // Ambiguous: qualify Printer::start or Scanner::start.
    return 0;
}
