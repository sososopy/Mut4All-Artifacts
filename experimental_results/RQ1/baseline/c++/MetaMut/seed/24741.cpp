
template<class T> concept C = T::value;

template<class T>
struct A {
  template<class U> requires requires { { T() } -> C; }
  friend void f(A) { }
};

template struct A<int>;

