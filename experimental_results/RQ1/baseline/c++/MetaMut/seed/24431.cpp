
template <class T>
struct Identity {
  typedef T type;
};

struct Foo {
  template <typename T>
  Foo(T*, void (Identity<T>::type::*m)(void));

  template <typename T>
  Foo(const T*, void (Identity<T>::type::*m)(void) const);
};

struct Bar {
  void Method(void) const;
};

void Bar::Method(void) const
{
  Foo foo(this, &Bar::Method);
}
