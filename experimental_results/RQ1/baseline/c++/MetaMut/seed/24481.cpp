
template<typename T>
T make()
{
  return { typename T::type{0} };
}

void f()
{
  struct X {
    using type = int;
    X(int) { }
  };

  make<X>();
}

