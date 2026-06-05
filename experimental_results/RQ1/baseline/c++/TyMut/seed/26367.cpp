
namespace std {
  template<typename T> struct tuple_size;
  template<int, typename> struct tuple_element;
}
struct A {
  template <int I> int& get() { return 1; }
};
template<> struct std::tuple_size<A> { static const int value = 3; };
template<int I> struct std::tuple_element<I,A> { using type = int; };
struct B {
  A *begin();
  A *end();
};
void f (B a)
{
  #pragma omp for collapse(2)
  for (auto [i, j, k] : a)
    for (int l = i; l < j; l += k)
      ;
}

