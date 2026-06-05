
struct token {};

struct example {};

template< typename >
struct helper
{
    static constexpr auto c() { return 42; }
};

struct impostor_c
{
    template< typename T, auto= helper< T >::c >
    static example func();
};

example c= impostor_c::func< token >();


