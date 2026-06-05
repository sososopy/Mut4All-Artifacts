
template<int size>
struct Name {};
template <class T> constexpr Name<5> tname5 {};

template <Name n> struct Use{};


Use<tname5<int>> use0;//ok

template <class T> Use<tname5<T>> use1;
template <class T> Use<tname5<int>> use2;
template <int> Use<tname5<int>> use3;

template <class T> struct Tmpl
{
    Use<tname5<T>> use0;
    Use<tname5<int>> use1;
};

template <auto v> struct Tmpl2
{
    Use<tname5<decltype(v)>> use0;
    Use<tname5<int>> use1;
};

template <class> void f()
{
    Use<tname<int>> use0;
};
