
#include <iostream>
#include <typeinfo>

template <typename T1, typename T2>
void f(T1 & t1, T2 & t2)
{
        auto a1 = t1, a2 = t2;
        std::cout << typeid(a1).name() << std::endl;
        std::cout << typeid(a2).name() << std::endl;
}

int main ()
{
        int i(23);
        double d(42.);
        f(i, d);
}
