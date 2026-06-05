
struct A
{
    A(int i) {}
};

struct B : virtual A
{
    using A::A;
};

void f()
{
    B b{5};
}

