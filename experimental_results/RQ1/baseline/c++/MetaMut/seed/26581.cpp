
#include <concepts>

template <typename T1, typename T2>
concept weakly_eq_comparable = requires (T1 t1, T2 t2) { { t1 == t2 } -> std::convertible_to<bool>; };

template <typename ...Ts>
struct my_tuple
{

    template <typename ...T2s>
    requires ((sizeof...(Ts) == sizeof...(T2s)) && (weakly_eq_comparable<Ts, T2s> && ...))
    friend bool operator==(my_tuple const & lhs, my_tuple<T2s...> const & rhs)
    {
        return true;
    }
};

int main()
{
    my_tuple<int, float> m1;
}
