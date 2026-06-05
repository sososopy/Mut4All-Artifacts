
struct A
{
  template<int> void foo() = delete;
};

template<int> void A::foo() { int i; }

template void A::foo<0>();
