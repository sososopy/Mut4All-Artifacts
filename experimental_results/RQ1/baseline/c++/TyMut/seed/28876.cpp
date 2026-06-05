
template<auto>struct X{};
template<auto V>requires requires{V.a;}struct X<V>{static constexpr bool v{false};};
template<auto V>requires requires{V.b;}struct X<V>;
struct A{int a;};
static_assert(!X<A{}>::v);
struct B{int b;};
template<auto V>requires requires{V.b;}struct X<V>{static constexpr bool v{true};};
static_assert(X<B{}>::v);
