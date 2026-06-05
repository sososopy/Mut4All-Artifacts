
#include <iostream>
struct S {
    template <typename T>
    void operator+(T) { std::cout << "1" <<"\n"; }
};
namespace N {
    template <typename T>
    void func(const T value1) { S s; s +(value1); }
}

struct S1 {};
namespace N {
    void operator+(S&, S1) { std::cout << "2" <<"\n"; }
}

int main(int, char**)
{
    S1 s1;
    N::func(s1);
}

