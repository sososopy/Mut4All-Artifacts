
enum class Foo
{
    A1
};

enum class Bar
{
    B1
};

template <auto EnumVal> struct enum_
{
};

template<class...> struct list {};

struct enum_type_map: list<enum_<Foo::A1>, int>, list<enum_<Bar::B1>, double> {};

template<class V> V f( list<enum_<Bar::B1>, V> const& )
{
    return {};
}

int main()
{
    f( enum_type_map() );
}
