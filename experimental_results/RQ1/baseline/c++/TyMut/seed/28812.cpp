
template<class T>
struct X {};

class Foo {
    template<class...>
    static constexpr inline bool bar = false;
    template<class T>
    static constexpr inline bool bar<X<T>> = true;
};
