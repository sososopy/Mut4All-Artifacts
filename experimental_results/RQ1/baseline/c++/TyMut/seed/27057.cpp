

#include <iostream>

template <typename N>
struct A {
    template <typename T>
    void operator+(T const&) { 
        std::cout << "member\n"; 
    }
};

template <typename N>
void operator+(A<N>&, int const& ) { 
    std::cout << "free\n"; 
}

int main()
{
    A<int> a;
    a + 2;
}
