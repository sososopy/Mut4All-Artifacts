
template <class T> struct foo
{
  template <typename U, typename=void>
  static void bar(const U&) {}

  static void bar(int x) { bar<int>(x); }
};

