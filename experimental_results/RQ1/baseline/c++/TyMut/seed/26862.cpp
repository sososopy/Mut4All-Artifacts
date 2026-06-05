
template<class Derived>
struct S
{
  void m() {
  }

  S()
  {
    if (&S<Derived>::Unwrap != &Derived::Unwrap)
      m();
    if (!(&Derived::UnwrapImpl))
      m();
  }

  void Unwrap() {
  }
};

struct T : public S<T>
{
  void UnwrapImpl() {
  }
};

T t;
