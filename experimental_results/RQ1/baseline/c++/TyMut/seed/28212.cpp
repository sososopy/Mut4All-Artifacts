
  struct tag_foo { } inline constexpr foo;
  struct tag_bar { } inline constexpr bar;

  template<typename... T>
  auto f(tag_foo, T... x)
  {
    return (x + ...);
  }

  template<typename... T>
  concept fooable = requires (T... x) { f(foo, x...); };

  template<typename... T> requires (fooable<T...>)
  auto f(tag_bar, T... x)
  {
    return f(foo, x...);
  }

  auto test()
  {
    return f(bar, 1, 2, 3);
  }

