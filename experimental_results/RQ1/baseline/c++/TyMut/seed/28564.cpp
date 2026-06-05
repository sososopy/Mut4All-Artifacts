
template<void(*)() = []{ return; }>
struct foo {};

template<typename T>
inline constexpr auto foo_v = foo<>{};

auto x = foo_v<long>;
