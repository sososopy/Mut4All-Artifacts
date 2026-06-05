
#include <optional>

struct Bar
{
    int size_;
    Bar( int size )
      : size_( size )
    {}
};

template<class T>
Bar get( T const& val )
{
  return Bar( __builtin_strlen(val) );
}

class Foo
{
    int size2_;
  public:
    Foo()
    {}

    template<class T>
    Foo( T const& t )
      : size2_( get<T>( t ).size_ )
    {}
};

enum Enum
{};

bool parseImpl2( Foo s, Enum* val )
{
  *val = Enum();
  for(;;)
  {
    s = "aa";
    if( true )
      return false;
    return true;
  }
}

template<class T> std::optional<T> parse2( Foo str )
{
  T res = T();
  if( parseImpl2( str, &res ) )
    return res;
  return std::optional<T>();
}

Enum transform()
{
  if( auto r = parse2<Enum>( Foo() ) )
    return *r;
  return {};
}

