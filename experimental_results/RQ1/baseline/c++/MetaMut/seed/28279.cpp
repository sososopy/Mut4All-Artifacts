
struct T
{
  template<int>
  void foo() const {}
};

template<typename T>
struct pair
{
  T a, b;
};

void bar()
{
  [](auto)
  {
    pair<T> x[10];
    for (auto const &[a, b] : x) a.foo<0>();
  };
}

