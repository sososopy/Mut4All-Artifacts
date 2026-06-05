
struct A
{
  void foo(auto);
};

void A::foo(auto) {}

template<typename> struct B
{
  template<typename T> void bar(auto);
};
