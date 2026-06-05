

template <class> struct A {
  virtual ~A();
};
template <class... Ts> struct B : A<Ts>... {
  ~B() = default;
};
struct C : B<int, char> {
  C() {}
};
