
typedef void (*foo_t)();

void test(foo_t)
{}

template< typename >
struct A
{
  template< typename = void > static void foo();

  void bar()
  {
    test(foo);
  }

  template< typename >
  void baz()
  {
    test(foo);
  }
};

template< typename _T_ >
template< typename >
void A< _T_ >::foo()
{}
