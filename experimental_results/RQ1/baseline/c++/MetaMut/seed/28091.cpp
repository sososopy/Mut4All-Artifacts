
struct A {
  int b;
};

struct B {
  A g;
};

const auto j = A{};

template <typename>
void k()
{
  B{j};
}
