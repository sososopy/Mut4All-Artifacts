template<class T, class U>
concept C = true;

template <class T>
struct Foo
{
  template <C<typename T::value_type> U>
  void bar()
  {
  }
};

Foo<float> f;

