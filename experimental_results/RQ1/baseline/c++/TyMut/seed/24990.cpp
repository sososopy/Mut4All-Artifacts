

#include <iostream>

struct X {
    X(std::initializer_list<int >) { std::cout << '1'; }
    X(X const& ) { std::cout << '2'; }
    operator int() { return 0; }
};

int main() {
    X x{1};
    X y{x};
}

