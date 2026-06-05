
#include <new>
struct A{
    int m{};
};
template<class T>
void f(A *instance, T A::*member_pointer)
{
    new (&(instance->*member_pointer)) T{0};
}

int main()
{
    A a;
    f(&a, &A::m);
    return 0;
}
