
#include <typeinfo>

int main() {
    constexpr auto v1 = &typeid(int);
    constexpr auto v2 = &typeid(char);
    static_assert( v1 != v2 );
} 

