
struct A {
  template <class>
  static constexpr bool val = true;
};

template <class T>
concept bool C = A::val<T>;

C{T} struct B {};
