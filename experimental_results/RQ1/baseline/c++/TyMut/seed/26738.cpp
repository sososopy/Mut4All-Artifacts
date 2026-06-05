
#include <type_traits>

template< class T >
struct Tmpl
{
  ~Tmpl() noexcept( std::is_nothrow_destructible< T >::value )
  {}
};

struct Empty
{
};

struct Foo
{
  ~Foo()
  {}

  Tmpl< Empty > member;
};

int main()
{
}

