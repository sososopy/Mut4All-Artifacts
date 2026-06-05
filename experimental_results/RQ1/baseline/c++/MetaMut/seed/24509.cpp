
template<typename> struct A;

template<> struct A<auto>
{
  template<int> void foo();
};

void bar()
{
  A<auto>().foo<0>();
}
