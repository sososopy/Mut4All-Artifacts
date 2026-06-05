
    #include <concepts>

    auto f(std::integral auto x) {}
    auto g(auto x) requires requires { { x }->std::integral; } {}

    auto main()->int {
        f(42); // OK
        g(42); // error
    }


