
template<typename T> struct S { };
template<> struct S<void(*)()> { };
template<> struct S<const void(*)()> { };

template<typename T, typename U> constexpr bool is_same_v = false;
template<typename T> constexpr bool is_same_v<T, T> = true;

static_assert( ! is_same_v< void(*)(), const void(*)() > );

