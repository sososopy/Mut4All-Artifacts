#include <iostream>

enum struct foo_t : int32_t
{
    A = -1,
    C
};

int main()
{
    static_assert( foo_t::A < foo_t::C, "" ); // < --- ok
    std::cout << ( foo_t::A < foo_t::C ) << std::endl; // <--- print 0
}
