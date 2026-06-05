
#include <type_traits>
template <typename T>
struct Key {
  Key(int);
};

class Forward {};

template <typename T>
constexpr bool C =
    std::is_same<T, Forward>(); // changing `Forward` to `double` will make the following static_assert passed.

template <typename Z>
struct Outer {
  template <typename U>
  struct Foo {
    Foo(U);
    U u;
  };

  template <typename V>
    requires(C<Z>)
  Foo(V) -> Foo<int>;
};

template <typename Y>
struct T {
  template <typename Y2>
  struct T2 {
    template <typename K>
    using AFoo = Outer<Y2>::template Foo<K>;
  };
};

T<Forward>::T2<Forward>::AFoo a{1.0};  // the explict deduction guide should be choosen, Foo<int>
static_assert(std::is_same<decltype(a), Outer<Forward>::Foo<int>>()); // expect to be true!
