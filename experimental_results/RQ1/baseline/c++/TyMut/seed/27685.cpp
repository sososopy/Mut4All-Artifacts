
    struct S {
        S(long double) {}
    };
    std::variant<double, S> v;
    v = 1.0L;
    assert(v.index() == 1);
