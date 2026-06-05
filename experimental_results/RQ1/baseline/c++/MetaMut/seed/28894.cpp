
struct B {
  void foo();
};

struct D : B {
  void show();
};

void
D::show()
{
  constexpr void *p = nullptr;
  if (p)
    static_cast<D *>(p)->foo();
}
