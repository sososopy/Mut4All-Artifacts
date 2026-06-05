
constexpr bool f(const char*) { return true; }
template<typename T> const char c = "FOO"[f("BAR") ? 1 : 0];


