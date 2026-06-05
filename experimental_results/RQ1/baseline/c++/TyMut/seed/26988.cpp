
template <typename> struct plus;
template <typename> struct A {
  template <typename T> auto operator()(T);
} foldl;
void foo() { foldl<plus<int>>(0); }

