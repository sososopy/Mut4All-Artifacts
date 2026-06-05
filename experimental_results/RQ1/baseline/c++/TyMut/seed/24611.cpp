
#include <cassert>
struct Foo
{
    int line;
    Foo( int line = __builtin_LINE() )
      : line( line )
    {}
};

int main()
{
  assert( Foo().line == (new Foo)->line );
}
