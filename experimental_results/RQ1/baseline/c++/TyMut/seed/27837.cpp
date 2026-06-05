
// @file main.cpp
#include <iostream>

template <int collapseDepth>
class Foo{
public:
    void print() {
#pragma omp parallel for collapse(collapseDepth)
        for (int x = 0; x < 1000; x++) {
            for (int y = 0; y < 1000; y++) {
                std::cout << "x: " << x << " y: " << y << std::endl;
            }
        }
    }
};

int main() {
    Foo<2> fooInstance;
    fooInstance.print();
}
