
template <typename T>
inline
int field(T const&)
{
  return 0;
}

template <typename T>
struct Foo
{
  int field;
};

template <typename N>
inline
void useFoo()
{
  Foo<N> foo;


  for( ; foo.field < 10; )
  {
  }
}

int main()
{
  useFoo<int>();
  return 0;
}
