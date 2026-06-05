
template<int> struct A;

template<> struct A<0>
{
  template<typename, typename...> struct B
  {
    template<typename> int foo() {}

    int i = foo<int>();
  };
};
