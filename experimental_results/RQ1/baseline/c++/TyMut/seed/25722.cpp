
template<typename> struct A
{
  constexpr A(int) {}

  virtual void foo()
  {
    constexpr A<void> a(0);
  }
};
