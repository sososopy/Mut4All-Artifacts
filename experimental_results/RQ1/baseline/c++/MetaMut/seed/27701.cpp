
struct A {
  template <typename A>
  static const int a = 0;
};
struct B {
  template <typename B>
  static int foo ()
  {
    return B::a;
  }
};
int bar ()
{
  int i = B::foo<A> ();
}

