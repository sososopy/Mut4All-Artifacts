
struct less {
    static constexpr auto operator()(int i, int j) -> bool {
        return i < j;
    }

    using P = bool(*)(int, int);
    operator P() const { return operator(); }
};

static_assert(less{}(1, 2));
