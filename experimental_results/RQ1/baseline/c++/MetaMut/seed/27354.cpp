

template <typename T>
struct Foo {
  template <typename U>
  explicit(static_cast<U>(true)) operator Foo<U>();
};

template <typename T>
template <typename U>
Foo<T>::operator Foo<U>() {
  return {};
}

int
main ()
{
  Foo<float> a;
  Foo<int> b = a;
}
