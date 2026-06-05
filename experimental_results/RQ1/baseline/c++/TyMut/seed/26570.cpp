
template <typename T>
struct optional {
  template <typename U> explicit optional (U);
  template <typename U = T> void operator= (U);
};
struct T { int a; };
struct S
{
  void foo (int y) { x = { y }; }
  optional<T> x;
};
