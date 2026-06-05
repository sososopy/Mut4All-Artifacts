

template <int>
struct S {
  __attribute__((used)) S() noexcept(noexcept(this->foo()));
  void foo();
};

void
g ()
{
  S<1> s;
}
