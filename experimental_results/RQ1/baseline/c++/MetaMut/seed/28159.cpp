
#include <initializer_list>

struct A;
struct V
{
    V() = default;
    V(const A &);
    A make_a() const;
};

struct A
{
    A();
    A(const A &);
    A(A &&);
    A(std::initializer_list<V>);
};

void sink(A &);
void f()
{
    A a{ V().make_a() };
    sink(a);
}

