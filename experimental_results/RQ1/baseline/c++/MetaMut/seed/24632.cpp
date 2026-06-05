
template <class T, class U>
struct foo
{
};
template <class T>
struct baz
{
  class bar;
};

template <class T, class D>
struct baz<T>::bar : foo<int, D>
{
};

baz<int>::bar it;

