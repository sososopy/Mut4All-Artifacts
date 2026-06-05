
template <typename T>
class A {
  typedef decltype((T())[0]) t;
};
