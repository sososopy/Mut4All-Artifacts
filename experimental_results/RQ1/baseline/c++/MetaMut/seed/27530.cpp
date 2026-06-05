struct S {
  unsigned a;
  unsigned b;
#ifdef CTOR
  constexpr S(unsigned _a, unsigned _b) noexcept: a{_a}, b{_b} { }
#endif
};

template <S p>
void fnc()
{
}

template<S s> struct X { };

void f()
{
  fnc<{.a = 10, .b = 20}>();
  X<{.a = 1, .b = 2}> x;
}
