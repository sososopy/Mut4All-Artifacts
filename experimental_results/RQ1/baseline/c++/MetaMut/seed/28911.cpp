
struct S
{
    void f() noexcept {}
    S &g() noexcept(noexcept(f())) { f(); return *this; }
};

