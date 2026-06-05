template<typename T, typename U, int i>
concept True = true;

template<typename T, int I>
struct Template
{
    static constexpr int i = I;

    friend constexpr auto operator+(True<T, i> auto f)
    {
        return i;
    }
};

template<int I>
struct Template<float, I>
{
    static constexpr int i = I;

    friend constexpr auto operator+(True<float, i> auto f)
    {
        return i;
    }
};

Template<void, 4> f{};

static_assert(+Template<float, 5>{} == 5);