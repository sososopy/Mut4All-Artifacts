
template <typename>
concept C = true;

template <C... Ts>
class Foo
{
private:
  Foo(const Ts&...) {};

public:
  friend auto factory(const C auto&...);  // #1
};

auto factory(const C auto&... ts)  // #2
{
  return Foo{ts...};
}

int main()
{
  factory(5);
}
