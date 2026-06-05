

typedef void T();
static_assert(std::is_same<T, T const>::value, "");

