
template<class T, class Enable = void> struct CFooEnabler {};

struct CFoo
{
    template<class T> inline CFoo( T const& t, typename CFooEnabler<T>::Enable* = 0 );
    char const* data() const;
    int size() const;
};

template<> struct CFooEnabler<char const*>
{
    typedef void Enable;
    static char const* extract( char const* val );
};

template<class T, class Enable = void> struct FooEnabler {};
struct Foo
{
    template<class T> inline
    Foo( T const& t, typename FooEnabler<T>::Enable* = 0 );
    Foo( char const* str, int size );
};

template<> struct FooEnabler<CFoo>
{
    typedef void Enable;
    static Foo extract( CFoo val )     // make this a declaration and it works
    {
      return Foo( val.data(), val.size() );
    }
};

template<class T>
struct FooEnabler<T, typename CFooEnabler<T>::Enable> : FooEnabler<CFoo> {};

void func()
{
  char const* a = "";
  Foo xx( a );
}
