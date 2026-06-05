

template< class > struct remove_cv;
template< class _Tp> using __remove_cv_t = typename remove_cv< _Tp> ::type;

template< class _Tp, _Tp __v>
struct integral_constant {
    static constexpr inline _Tp value = (__v);
    typedef _Tp value_type;
    typedef integral_constant type;
    constexpr operator value_type() const noexcept { return value; }
    constexpr value_type operator()() const noexcept { return value; }
};

using false_type = integral_constant< bool, false> ;

template< class ...> struct __or_;

template<> struct __or_< > : public false_type {};


template< class , class > struct is_same;

template< class _Tp, class ..._Types> using __is_one_of = __or_< is_same< _Tp, _Types> ...> ;

template< class _Tp> using __is_signed_integer = __is_one_of< _Tp , 
    signed char, signed short, signed int, signed long, signed long long, signed __int128_t> ;

