
#include <type_traits>
#include <iostream>

struct A { int x; };
struct B { float y; };
struct C { A a; B b; };

int main()
{
#if __cpp_aggregate_paren_init >= 201902L 
    std::cout
        << std::is_constructible_v<C, A> << '\n'               // 1
        << std::is_constructible_v<C, A, B> << '\n'            // 1
        << !std::is_constructible_v<C, B> << '\n'              // 1
        << std::is_trivially_constructible_v<C, A> << '\n'     // 1
        << std::is_trivially_constructible_v<C, A, B> << '\n'; // 0, seems buggy
#endif
}

