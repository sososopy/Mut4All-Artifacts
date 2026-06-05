
struct Temp{ ~Temp(); };
struct A{ A(const Temp&) noexcept; };
struct B{ ~B(); };
struct Pair{ A a; B b; };

Temp make_temp() noexcept;
void foo(const Pair&) noexcept;

void bar(const Pair& p) noexcept
{
    foo({A(make_temp()), p.b});
}
