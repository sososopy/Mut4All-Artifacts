template <typename T> struct O {
  static const T v = 0;
};

struct P {
  template <typename T> using I = typename O<T>::v;
};

struct Q
{
  template <typename T>
  int foo() { return T::template I<int>; }
};

int bar() {
  Q m;
  return m.foo<P>();
}