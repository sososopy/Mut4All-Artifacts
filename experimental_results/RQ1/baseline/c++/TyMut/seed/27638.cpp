
template <typename> struct A;
template <typename T> struct A<T[]> { typedef T type; };
template <typename T> using U = typename A<T>::type;

struct B {
  template <typename T> B(T);
};

long int sz;

template <typename T> void foo() { new U<T>[sz](); }

struct D {
  explicit D();
};

struct C {
  B b;
  C() : b(foo<D[]>) {}
};

