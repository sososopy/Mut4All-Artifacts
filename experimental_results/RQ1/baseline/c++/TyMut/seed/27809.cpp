
#include <iostream>

struct ES { 
    enum E { v }; 

    // 1
    friend std::ostream& operator<<(std::ostream& os, E) { return os << "E"; }
};

// 2 - should be the same but...
// std::ostream& operator<<(std::ostream& os, ES::E) { return os << "E"; }

struct S {
    ES::E e : 1; 
};

int main() {
    S s{}; 
    std::cout << s.e << std::endl;
    return 0;
}
