
void f() {
  float x, &r = x;
  auto l = [=] () {
    static_assert(std::is_same_v<decltype(x), float>);
    static_assert(std::is_same_v<decltype((x)), const float&>);
    static_assert(std::is_same_v<decltype(r), float&>);

    // fails, decltype((r)) is float&
    static_assert(std::is_same_v<decltype((r)), const float&>);
  };
}
