
struct foo { };

template <const auto &> void fnc() { } 

void
test()
{
  static constexpr foo a;
  fnc<a>();
}
