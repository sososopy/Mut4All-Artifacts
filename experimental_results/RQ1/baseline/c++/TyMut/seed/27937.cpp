
    struct one {};

    struct two
    {
        two() { }
        two(one const &) { }
        operator one() const { return one{}; }
        two operator+(one const &) const { return two{}; }
    };

    two operator+(two const &, two const &) { return two{}; }

    template<typename T>
    auto operator+(T const &lhs, two const &rhs) -> decltype(rhs + lhs)
    {
        return rhs + lhs;
    }

    void test()
    {
        one o;
        two t;
        auto a = o + t;
    }
