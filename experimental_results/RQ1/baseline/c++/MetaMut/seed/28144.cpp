
#include <vector>
#include <cstdio>

struct Foo
{
    Foo () = default;

    Foo (Foo &&f)
      : x (f.x)
      , y (f.y)
    {
      f.y = 0;
    }

    int x = 123;
    int y = 456;
};

int main ()
{
  std::vector<std::pair<Foo, Foo>> v;
  for (size_t i = 0; i < 10; ++i)
  {
    v.emplace_back ();
  }

  for (const auto &p : v)
  {
    printf ("%d\n", p.first.y);
  }
}
