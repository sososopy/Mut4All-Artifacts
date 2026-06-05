
#include <iostream>

template <typename... Whatever>
class A;

template <typename Type>
class A<Type> {};

template <typename Type, typename Head, typename... Tail>
class A<Type, Head, Tail...> : A<Type, Tail...> {};

class B : public A<int, int, double> {};

template <typename Type, typename... Unused>
void f(A<Type, Unused...> *ref)
{
    std::cout << "f.a" << std::endl;
}

int
main()
{
    B *b = new B;
    f(b);

    return 0;
}

