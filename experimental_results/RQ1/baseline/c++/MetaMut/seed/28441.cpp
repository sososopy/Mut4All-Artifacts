
#include <iostream>

int g = 0;

struct A { 
    ~A() { g = 1; }
    consteval A() {}
};

int main() {
    A{};
    std::cout << g;
}
