
template<typename F>
struct Item
{
  F (*foo)();
};

template<typename F>
struct Container
{
  static const Item<F> items[1];
};


template<typename F>
const Item<F> Container<F>::items[1] =
{
  {
    []() -> F { return F(); }
  }
};

template struct Container<int>;

int main()
{
  return 0;
}

