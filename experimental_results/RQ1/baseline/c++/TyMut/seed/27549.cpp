
template <class T> [[noreturn]] void Throw( T const& t, char const*  /* f */ )
{ throw T(t); }

template <class U>
int DoIt()
{
    //::Throw( int(42), __FILE__ );    // No warning
    ::Throw( int(42), __FUNCTION__ );  // warning: no return statement
}

int main()
{
    DoIt<long>();
    return 0;
}

