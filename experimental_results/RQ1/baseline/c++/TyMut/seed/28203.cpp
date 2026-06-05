

template <typename T>
struct Foo {
  Foo operator+(Foo &&f) { return f; }
  T &x;
};
template <typename T> Foo(T) -> Foo<T>;

void zod()
{
  int x,y,z;
  (Foo{x} + Foo{y}) + Foo{z};
}
