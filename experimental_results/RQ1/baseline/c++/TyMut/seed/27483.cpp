
#include <concepts>

template <std::size_t N>
struct s { constexpr static auto const n = N; };

template <typename T>
concept S = requires (T t)
{
  { t.n } -> std::same_as <const std::size_t
#if NOBUG
&
#endif
>;
  requires std::is_same_v <decltype (t.n), const std::size_t>;
};

decltype (auto) f (S s) { return s.n; };

static_assert (std::is_same_v <decltype (f (s <1> {})), std::size_t>);
static_assert (S <s <1>>);

