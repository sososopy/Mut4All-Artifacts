
    template<bool ...Bs>
    struct and_ 
    {
        constexpr static bool value{true};
    };

    template<typename T>
    struct true_
    {
        constexpr operator bool() const { return true; }
    };

    template<typename ...Ts>
    constexpr bool foo(Ts...)
    {
        return and_<(true_<Ts>())...>::value;
    }

    int main()
    {
    }

