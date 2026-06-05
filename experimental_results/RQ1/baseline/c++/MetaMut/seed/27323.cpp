
constexpr bool any (bool) { return true; };
template <typename T> concept Any = requires (T t) { requires any (t); };
constexpr static int f (Any auto) { return 42; };
constexpr auto q = f (false);


