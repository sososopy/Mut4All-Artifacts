
typedef bool (*Function)(int);
constexpr bool check(int x, Function p) { p(x); }

static_assert(check(2, check));
