
union type {
    constexpr type(){}
    constexpr ~type() {}
    int t;
};

struct S
{
    constexpr S() = default;
    constexpr bool f() const { return true;}
private:
    type v[1];
};


static_assert(S{}.f());

