
template<typename> struct A
{
  template<typename> struct B {};
  A() { B<int> b; }
};

template<> template<typename>
struct A<int>::B
{
  virtual void foo() {}
};

A<int> a;
