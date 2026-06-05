
template <unsigned int Coordinate>
struct dimension{
    template<typename ... T>
    constexpr dimension(T...t){}
};

struct accessor_base{
    template<typename ... T>
    constexpr accessor_base(T...t){}
};

template <typename ArgType, typename Pair>
struct accessor_mixed{

private:
    static constexpr accessor_base s_args_constexpr{dimension<Pair::first>{0} };
};
