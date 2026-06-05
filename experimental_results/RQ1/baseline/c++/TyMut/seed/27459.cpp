
struct A {
    int x;
    char y;
};

template <class T>
struct B {
    [[no_unique_address]] T t;
    bool u;
};

static_assert(sizeof(B<A>) == sizeof(A));

