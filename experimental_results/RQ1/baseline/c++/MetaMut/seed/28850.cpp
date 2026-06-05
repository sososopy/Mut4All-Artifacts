
template <class T>
class Foo
{
    constexpr operator T() -> T {}
};

struct S {
  operator int() const -> double;
};

class A { operator auto*() -> int; };
