
template<int> struct A
{
  template<typename T> operator T();
};

void foo()
{
  A<0>().operator auto();
}
