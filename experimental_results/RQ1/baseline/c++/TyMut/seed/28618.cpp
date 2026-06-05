
    #include <type_traits>

    template <class T1, class T2>
    struct pair {
        template<class U1 = T1, class U2 = T2>
        constexpr pair(U1&&, U2&&) { }
    };

    struct BraceInit { BraceInit() = default; };
    struct ExplicitBraceInit { explicit ExplicitBraceInit() = default; };

    constexpr int f(pair<ExplicitBraceInit, ExplicitBraceInit>) { return 1; }
    constexpr int f(pair<BraceInit, BraceInit>) { return 2; }

    static_assert(f({{}, {}}) == 2, "");

