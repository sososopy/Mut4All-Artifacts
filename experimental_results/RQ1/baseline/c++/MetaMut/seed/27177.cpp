
#include <bits/stl_construct.h>

union S {
    int i;
};

constexpr int f() {
    S s;
    std::construct_at(&s.i, 0);
    return 0;
}

constexpr int z = f();

