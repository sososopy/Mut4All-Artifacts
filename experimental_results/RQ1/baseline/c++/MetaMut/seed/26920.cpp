
    struct base { };
    struct derived : base {
        constexpr derived& operator=(derived const&) = default;
    };

