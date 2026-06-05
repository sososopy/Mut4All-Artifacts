
    struct A {
        static constexpr auto x = 42;
    };

    auto f(auto a) requires (a.x > 10) {}

    auto main()->int {
        f(A{});
    }
