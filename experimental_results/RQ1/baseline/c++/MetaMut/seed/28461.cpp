
#include <iostream>
struct A
{
    int a;

    [[nodiscard]] constexpr bool operator==(const A&) const = default;
};

struct B
{
    A a;
    int b;

    [[nodiscard]] constexpr bool operator==(const B&) const = default;
};

struct C : B
{
    int c;

    [[nodiscard]] bool operator==(const C&) const = default;
};

struct D : B
{
    int d;

    [[nodiscard]] constexpr bool operator==(const D&) const = default;
};


std::ostream &operator<<(std::ostream &os, const A &a) {
    os << "A("<<a.a<<")";
    return os;
}
std::ostream &operator<<(std::ostream &os, const B &b) {
    os << "B("<<b.a<<", "<<b.b<<")";
    return os;
}
std::ostream &operator<<(std::ostream &os, const C &c) {
    os << "C("<<c.a<<", "<<c.b<<", "<<c.c<<")";
    return os;
}
std::ostream &operator<<(std::ostream &os, const D &d) {
    os << "D("<<d.a<<", "<<d.b<<", "<<d.d<<")";
    return os;
}


int main()
{
    A a1{1}, a2{2};
    B b1{a1,2}, b2{a2,2};
    C c1{b1,3}, c2{b2,3};
    D d1{b1,3}, d2{b2,3};

#define CHECK(x,y) std::cout << x << " == " << y << "? " << ((x==y) ? "true" : "false") << std::endl;
    CHECK(a1,a2);
    CHECK(b1,b2);
    CHECK(c1,c2);
    CHECK(d1,d2);
}
