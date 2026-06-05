
class foo {
  template <typename T> void bar(T v);
};
extern template void foo::bar(const void *);
