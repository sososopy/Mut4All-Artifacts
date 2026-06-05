
template <class T>
constexpr T test_increment(T t)
{
  return ++t;
}

int main()
{
  constexpr double t = test_increment(2.0f);
  static_assert(t == 3);
}

