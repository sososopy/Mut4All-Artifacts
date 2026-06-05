

struct f {
  static constexpr int d = 3;
  typedef int e;
};
template <int a> struct x { };
template <typename g, g j, g m> using n = x<j + m>;
template <typename ac> auto v() -> n<typename ac::e, 0, ac::d>;
void af() { v<f>(); }
