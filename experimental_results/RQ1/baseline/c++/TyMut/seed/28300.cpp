
template <int>
struct S {};

template <auto>
struct V {};

template <int I, S<I> s>
void foo(V<s>) {}

void bar() {
    constexpr S<1> s;
    foo(V<s>{});
}
