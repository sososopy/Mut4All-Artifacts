
struct empty { };

struct foo
{
  [[no_unique_address]] empty x;

  constexpr
  foo() : x{}
  { }
};

struct bar : foo
{
  using foo::foo;
};

constexpr bar a{};
