
struct S {
  template<typename T>
  void f(T) noexcept(B);
  static constexpr bool B = true;
};

