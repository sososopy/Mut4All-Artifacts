struct empty { };

struct foo
{
  [[no_unique_address]]
  empty x;

  constexpr
  foo() : x{}
  { }
};

struct bar : foo
{
  using foo::foo;
  int i;
  constexpr bar() : i{1} {}
};

constexpr bar a{};

