#include <string>

struct Foo;

extern Foo &get_foo_by_name(const std::string &name);

const Foo &bug(bool x)
{
  const Foo &f = get_foo_by_name(x ? "x" : "y");
  return f;
}

