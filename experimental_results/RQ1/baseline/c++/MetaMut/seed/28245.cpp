
auto f(auto x) {
  auto [y] = x;
  return y;
}
int i = f(0);
