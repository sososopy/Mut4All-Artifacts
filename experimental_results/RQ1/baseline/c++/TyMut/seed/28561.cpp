
    #include <concepts>
    
        template <class T>
        struct
    wrapper
    {
        T value;
    };
    
        template <class T, class U>
        requires std::equality_comparable_with <T, U>
        constexpr bool
    operator == (wrapper <T> const& a, wrapper <U> const& b)
    {
        return a.value == b.value;
    }
        template <class T, class U>
        requires std::equality_comparable_with <T, U>
        constexpr bool
    operator == (wrapper <T> const& a, U const& b)
    {
        return a.value == b;
    }
        template <class T, class U>
        requires std::equality_comparable_with <T, U>
        constexpr bool
    operator == (T const& a, wrapper <U> const& b)
    {
        return a == b.value;
    }
    
        constexpr auto
    a = wrapper <int> { 2 };
        constexpr auto
    b = a;
    
    static_assert (a == b);
    
    int main () {}

