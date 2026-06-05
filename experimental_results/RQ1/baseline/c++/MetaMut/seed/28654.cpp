
template<auto> constexpr char v{};
template<> constexpr char v<2>{};
template<> constexpr char v<3>{};
static_assert(&v<0> != &v<1>);
static_assert(&v<2> != &v<3>);
