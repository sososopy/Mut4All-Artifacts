
#include <cstdio>
#include <cstdint>

struct Foo
{
  Foo(std::int64_t)
  {
  }
};

void foo(const char*)
{
  std::puts("In foo(const char*)");
}

void foo(const Foo&)
{
  std::puts("In foo(const Foo&)");
}

int main()
{
  foo((int)0);
  foo((unsigned)0);
  foo((short)0);
  foo((unsigned short)0);
  foo((std::int64_t)0);
  foo((std::uint64_t)0);
  foo(0);
  foo(NULL);
  foo(nullptr);
}

