

#include <utility>

struct static_for_result
{
    template <typename... Ts>
    constexpr decltype(auto) operator()(Ts&&... xs)
    {
        auto fn_call = [this, &xs...]()
        {
            return ([](auto&&...)
                {
                })(std::forward<decltype(xs)>(xs)...);
        };

        return int{};
    }
};

template <typename TF>
constexpr decltype(auto) static_for(TF&&)
{
    return static_for_result{};
}

int main()
{
    auto empty_for = static_for([&](auto, auto)
        {
        })(0);

    return 0;
}
