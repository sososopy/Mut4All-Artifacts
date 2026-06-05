extern "C" int printf(const char*, ...);

struct B { };

struct X : public B { long p; };

struct Y : public B { X q; };

Y foo( Y& y )
{
    Y temp;
    temp = y;
    return temp;
}

int main( void )
{
    printf("%zd %zd\n", sizeof( X ), sizeof( Y ));

    Y y;
    y.q.p = 6L;

    Y yy = foo( y );
    printf("%ld\n", yy.q.p);
}

