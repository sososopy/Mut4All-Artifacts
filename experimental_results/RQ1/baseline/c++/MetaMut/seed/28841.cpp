
template<class> concept Never = false;
template<class T> concept C = Never<typename T::type>;
void f() {
  struct X {
    using type = int;
  };
  static_assert(not C<X>);
}
