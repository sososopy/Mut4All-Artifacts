
struct A {
  constexpr int operator()() const { return 0; }
};
template <typename... T>
constexpr int sink(T...) {
  return 0;
}
template <int... N>
constexpr int run() {
  return sink(A()() + N...);
}
constexpr int k = run<1, 2, 3>();

