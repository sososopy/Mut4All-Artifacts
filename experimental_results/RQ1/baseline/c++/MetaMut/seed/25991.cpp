
typedef int Int;

template<class T, class U> struct is_same       { enum { value = 0 }; };
template<class T>          struct is_same<T, T> { enum { value = 1 }; };

struct X
{
    operator int() const { return 0; }
    operator Int() const { return 0; }
};

static_assert( is_same<Int, int>::value, "Int is int" );
