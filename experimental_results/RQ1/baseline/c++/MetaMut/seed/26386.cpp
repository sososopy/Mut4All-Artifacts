
template<typename T>
concept C1 = __is_same_as(T, int);

template<typename T>
concept IsNotTiny = !__is_same_as(T, char);

void
foo ()
{
  auto g = []<C1 T> requires IsNotTiny<T>(T t) -> T
    requires IsNotTiny<decltype(t)> { return t; };

  [](auto t) requires true { return t; }(5);
  [](C1 auto t) { return t; }(5);

  auto a0 = [](IsNotTiny auto a) { return [](auto b){ return b; }; };
  auto a1 = a0(1);
}
