
struct S2
{
    constexpr S2() = default;
    constexpr S2(S2 const &) = default;
    constexpr S2(S2 &&) = default;
    constexpr S2& operator=(S2 const &) = default;
    constexpr S2& operator=(S2 &&) = default;

    constexpr S2 (int const &) {}
};

struct S
{
    operator int()
    {
        return 3;
    }

    operator S2()
    {
        return S2{};
    }
};

int main()
{
    S2 s = static_cast<S2>(S{});
}
