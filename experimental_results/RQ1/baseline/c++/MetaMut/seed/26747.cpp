namespace std {
  template <class> class initializer_list;
}
struct A {
  template <typename... B> A(std::initializer_list<B...>);
};
A a { 0 };

