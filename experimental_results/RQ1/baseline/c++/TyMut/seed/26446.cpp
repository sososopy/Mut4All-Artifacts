
using size_t = decltype(sizeof(0));
template<typename T> struct type_identity { using type = T; };
template<typename T, typename U> constexpr bool is_same_v = false;
template<typename T> constexpr bool is_same_v<T, T> = true;

template<typename... Ts>
struct type_list {};

template <size_t I, typename T>
struct indexed { using type = T; };

template <size_t I, typename ...Ts>
struct indexer;

template <size_t I, typename T, typename ...Ts>
struct indexer<I, T, Ts...> : indexed<I, T>, indexer<I+1, Ts...> {};

template <size_t I, typename T>
struct indexer<I, T> : indexed<I, T> {};

template <size_t I, typename ...Ts>
using nth_element_t = typename decltype([]<typename T>(const indexed<I, T>&){return type_identity<T>{};}(indexer<0, Ts...>{}))::type;

template<size_t I, typename T>
struct nth_type;

template<size_t I, typename ... Ts>
struct nth_type< I, type_list<Ts...> >
{
#ifndef USING
  typedef nth_element_t<I, Ts...> type; // Incorrect result with it
#else
  using type = nth_element_t<I, Ts...>; // Compile error
#endif
};

template<size_t I, typename T>
using nth_type_t = typename nth_type<I, T>::type;

using list = type_list<int, float, double>;

using elm0 = nth_type_t<0, list>;
using elm1 = nth_type_t<1, list>;
using elm2 = nth_type_t<2, list>;

static_assert( is_same_v<elm0, int> );
static_assert( is_same_v<elm1, float> );
static_assert( is_same_v<elm2, double> );

