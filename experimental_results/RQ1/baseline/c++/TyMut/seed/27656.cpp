
template<class>
struct A {
    struct B {};
};

template<class T>
concept C = requires (T&& t) {
    t.a;
};

static_assert(C<A<int>::B>);
