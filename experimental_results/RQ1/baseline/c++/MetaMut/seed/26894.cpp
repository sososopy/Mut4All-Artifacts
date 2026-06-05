

struct Any
{
  template <class T>
  Any( T const & ){}
};

template <class T>
void func( T t, Any a = [](){} ){}

template void func<int>( int, Any );
template void func<double>( double, Any );

int main()
{
  func( 3 );
  func( 3.0 );

  return 0;
}

