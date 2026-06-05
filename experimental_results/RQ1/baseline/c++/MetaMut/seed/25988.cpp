
template<typename T, template <typename> class U> void foo(const U<T>&) {}

template<typename> struct A
{
  using B = A;
  B bar();
};

void baz()
{
  foo(A<int>().bar());
}
