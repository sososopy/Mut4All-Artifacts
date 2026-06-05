
template<typename = void>
struct B {
  constexpr int f() requires true { return 5; }
};

template<typename = void>
struct S : B<> {
  using B::f;
  constexpr int f() { return 10; }
};

static_assert(S<>{}.f() == 5);

