
struct Y { };

          bool operator<(Y a, Y b) { return false; }
constexpr bool operator>(Y a, Y b) { return false; }

static_assert(!noexcept(Y{} > Y{}));
static_assert(!noexcept(Y{} < Y{}));

