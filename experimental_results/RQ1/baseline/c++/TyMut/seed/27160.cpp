
struct S { int i, j; };

struct A {
  S s;
  constexpr A(S e) : s(e) {}
};

void
f()
{
  A g[1]({{1, 1}});
}
