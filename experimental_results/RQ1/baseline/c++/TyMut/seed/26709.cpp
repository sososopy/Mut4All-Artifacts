
#include <iostream>

struct A {
    explicit operator int() {
        return 42;
    }
    
    operator float() {
        return 123.0f;
    }
    
    operator double() {
        return 456.0;
    }
};

int main() {
    int i = A(); // note copy-initialization
    std::cout << i << std::endl;
}
