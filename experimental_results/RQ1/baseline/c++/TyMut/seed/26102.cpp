

template < typename T >
struct S
{
    T t ;
    S() : t(0) { } } ;
} ;

// local class defined inside template definition.
template < typename T >
void f()
{
    struct local { local( int ) { } } ;

    S<local> s ;
}

// local class defined inside non-template definition.
void g()
{
    struct local { local( int ) { } } ;

    S<local> s ;
}


int main()
{
    f<void>() ; // error: no matching constructor for initialization of 'local'
    g() ; // OK, lookup works
}

