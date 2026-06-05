
template<int N> struct A
{
  A* p = new A<N>;
};
