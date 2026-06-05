
template <auto>
struct A {
    static constexpr bool value = false;
};

template <auto p>
  requires __is_same(decltype(p), int*)
struct A<p> {
    static constexpr bool value = false;
};

int x = 0;
//static_assert( A<&x>::value );
static_assert( A<const_cast<const int*>(&x)>::value == false );

