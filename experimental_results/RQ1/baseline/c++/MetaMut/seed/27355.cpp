
template<typename Lambda>
constexpr void test(Lambda lambda)
{
    constexpr auto result = lambda();
    static_assert(result.value == 1);
}

struct teststruct
{
    int value = 0;
};

constexpr auto get_value()
{
    teststruct result{};
    result.value = 1;
    return result;
}

int main()
{
    constexpr auto value = get_value();
    //Here's a lambda without any captures
    auto options_lambda = []() { return value; };
    test(options_lambda);
}
