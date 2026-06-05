
struct foo_t {
    template<class T>
        static constexpr bool bar = true;
};
inline constexpr foo_t foo{};

template<class T>
    struct baz {
        static_assert(foo.bar<T>); // bug in clang (before instantiation) and gcc (during instantiation)
        static_assert(foo_t::bar<T>); // OK
    };

int main()
{
    static_assert(foo.bar<void>, ""); // OK
    static_assert(foo_t::bar<void>, ""); // OK
    static_cast<void>(baz<void>{});
}
