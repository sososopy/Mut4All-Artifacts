
struct s { static constexpr bool v = true; };
template<auto> inline constexpr bool b = true;
constexpr bool f(auto x) requires b<x.v> { return true; }
static_assert(f(s{})); // clang ok, gcc nope, msvc ok

