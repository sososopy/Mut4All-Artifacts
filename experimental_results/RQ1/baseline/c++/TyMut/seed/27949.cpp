
template<int n>
struct number {
  constexpr /*implicit*/ operator int() const { return n; }
  using type = number<n>;
};

using false_ = number<0>;
using true_ = number<1>;

static_assert(!false_{}, "");
static_assert(true_{}, "");

template<int... ns> using numbers = list<number<ns>...>;

