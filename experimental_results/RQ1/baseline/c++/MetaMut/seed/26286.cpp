
struct foo {
    union
    {
        int x;
        short y;
    };
    constexpr foo() : x( 0 ) { }
};
