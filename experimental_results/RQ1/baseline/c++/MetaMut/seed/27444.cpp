
template <typename T>
struct B {
  void foo(T t) {}
};

template <typename T>
struct D : B<T> {
  void foo(T t) noexcept(noexcept(B<T>::foo(t))) {}
};

template struct D<int>;
