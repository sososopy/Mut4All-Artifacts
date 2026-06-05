
template <typename> struct A {};
template <typename> struct B {};
template <typename> struct C {
  void foo () { B a = A<int> { foo }; }
};
