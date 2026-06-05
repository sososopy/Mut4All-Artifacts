struct Y
{
    void swap(Y& rhs) noexcept {}
};

template<class B>
struct D : B
{
    void swap(D& rhs) noexcept (noexcept(B::swap(rhs))) {}
};

struct E : Y
{
    void swap(E& rhs) noexcept (noexcept(Y::swap(rhs))) {}
};

void foo()
{
    D<Y> x{}, y{};
    x.swap(y); // Rejected

    E u{}, v{};
    u.swap(v); // OK
}

