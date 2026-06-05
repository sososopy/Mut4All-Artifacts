
template<typename> struct A
{
  template<typename T> A(T, int = []{ return 0; }()) {}
};

A<int> a = 0;
