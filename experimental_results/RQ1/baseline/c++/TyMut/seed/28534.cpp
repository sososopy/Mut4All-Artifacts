
struct B {
    bool b = true;
    int i;
    consteval B() {}
};
static_assert( B{}.b );
