struct A {
    constexpr operator int() const { return 0; }
};

int main() {
    A a{};
    constexpr int b = a;             // ok
    [a]() { constexpr int b = a; };  // ok
    
    // error: the value of 'a' is not usable in a constant expression
    [&a]() { constexpr int b = a; }; 
}

