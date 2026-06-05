
//test.cpp

#include <iostream>

class A {
public:
    A() {std::cout << "A test" << std::endl;}
};

static const A a;

int main() {
    return 0;
}


//test2.cpp

#include <iostream>

class A {
public:
    A() {std::cout << "A test2" << std::endl;}
};

static const A b;

