
    struct array {
        int elems_[1];
    };
    
    constexpr array a{{0}};
    static_assert(a.elems_ != a.elems_ + 1, "");
