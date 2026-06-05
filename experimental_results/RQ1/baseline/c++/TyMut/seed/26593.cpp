
template <class X>
struct A {
    constexpr virtual X a() const = 0;
};

template <class X>
struct B {
    constexpr virtual X b() const = 0;
};

template <class X>
struct AB : public A<X>, public B<X> {
    constexpr X a() const override { return {}; };
    constexpr X b() const override { return {}; };
};

int main() {
    using x_type = bool;
    constexpr AB<x_type> ab{};
    static_assert(ab.a() == x_type{});
    static_assert(ab.b() == x_type{});

    return 0;
}

