

// foo.cpp
int var = 10;                // external linkage

// main.cpp
#include <iostream>

static int var = 100;        // internal linkage

int main() {
    extern int var;          // internal linkage
    std::cout << var << std::endl;
    {
        extern int var;      // g++: external linkage , clang++: internal linkage
        std::cout << var << std::endl;
        {
            extern int var;  // g++: external linkage , clang++: internal linkage
            std::cout << var << std::endl;
        }
    }
}    

