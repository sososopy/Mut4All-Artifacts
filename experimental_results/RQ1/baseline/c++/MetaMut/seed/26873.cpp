struct C
{
    template<typename T, typename... TRest>
    auto operator()(T&& x, TRest&&...){ return x; }
};

int main()
{
    // Compiles both with clang and gcc.
    auto c = C{};
    c(1);

    // Compiles with clang 3.7.
    // Does not compile with gcc 5.2.
    auto l = [](auto&& x, auto&&...) { return x; };
    l(1);
}
