
union foo { int i; double d; };

constexpr int const* addr(foo const& f)
{ return &f.i; }

constexpr int const& ref(foo const& f)
{ return f.i; }

int main()
{
    constexpr foo f { 42 };
    // Fine
    static_assert( &f.i, "" );
    // Not fine
    static_assert( addr(f), "" );
    // Not fine
    static_assert( &ref(f), "" );
}

