
template<int __v> struct integral_constant { static const int value = __v; };
template<bool, class> struct enable_if;
template<bool _Bp, class _Tp> using __enable_if_t = enable_if<_Bp, _Tp>;
template<class...> using void_t = void;
template<class... _Pred> void_t<__enable_if_t<_Pred::value, void>...> foo();
using type = decltype(foo<integral_constant<1>>());

