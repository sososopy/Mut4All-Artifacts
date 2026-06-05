
template <typename> struct A {
  struct B {} *b;
  static const int c { sizeof (b) / sizeof (b[0]) };
};
A<int> c;
const int d = A<int>::c;
