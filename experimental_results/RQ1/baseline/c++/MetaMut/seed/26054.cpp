
template<typename T> struct A
{
  typedef T B [[mode]];
};

A<int>::B b;
