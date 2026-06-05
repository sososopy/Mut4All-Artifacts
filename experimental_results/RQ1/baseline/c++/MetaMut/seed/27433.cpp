
template <bool V> struct A {
  template <class T> struct B { B(T) requires V; };
};

A<true>::B x(0);

