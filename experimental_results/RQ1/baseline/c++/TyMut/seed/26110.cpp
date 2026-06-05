
#include <iostream>

namespace N
{
    struct A
    {
        int operator+(const void*) 
        { 
            return 42; 
        }
    };
}

namespace M
{
    struct B
    {
    };
}


template <typename T, typename U>
int add(T t, U u)
{
    return t + u;
}

int operator+(N::A, M::B*) 
{ 
    return 5; 
}

int main(int argc, char** argv)
{
    N::A a;
    M::B b;
    std::cout << add(a, &b) << "\n";
}

