
template <typename T>
class X {
  template <typename S> void X<S>::f() __attribute__((locks_excluded()));
};
