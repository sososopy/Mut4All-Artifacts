
constexpr bool foo(const char *, char *, long) { return true; }
constexpr bool foo(const char *, char *, unsigned) { return true; }
constexpr bool foo(char, char, long) { return true; }
constexpr bool foo(char, char, unsigned) { return true; }

static_assert(foo((char *)nullptr, nullptr, 0));
