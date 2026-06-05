
struct B {
    int b;
    friend constexpr auto operator<=>(const B&, const B&) = default;
};

void foo()
{
  B b1, b2;
  b1 == b2;
}
