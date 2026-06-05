
template < typename T >
    requires requires
{
    // ICE
    []{ T::value ; } ;
}
void f()
{ }

struct S { inline static int value = 0 ; } ;

int main ()
{
    // ill-formed.
    f< int >() ;
    // should be well-formed?
    f< S >() ;
}

