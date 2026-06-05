
#include <utility>

struct A {
    A() { }
    A(A const&) = delete;
    A(A&& ) = delete;
};

struct B {
    A a;
    
    B() = default;
    B(B const& ) : a() { }
    B(B&& ) = default;
};

int main()
{
    B b1;
    B b2(std::move(b1));
}

