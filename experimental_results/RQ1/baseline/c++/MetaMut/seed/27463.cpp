
template<typename T, typename U>
constexpr bool is_same_v = false;

static_assert(is_same_v<bool, decltype(requires { requires false; })>);

