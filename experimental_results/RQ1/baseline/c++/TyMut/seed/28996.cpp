
template <int &R>
struct A {
  static constexpr int &value = R;
};

template <typename = void>
auto
S ()
{
  static int s;
  return A<s>{};
}

auto s = decltype (S ())::value;

