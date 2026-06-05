

#include <iostream>
#include <type_traits>

int main() {
    struct foo { volatile int x; };
    std::cout << std::is_trivially_copyable<foo>::value << std::endl;
}

