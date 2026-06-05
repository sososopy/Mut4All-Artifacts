
struct g
{
  char a;
};
constexpr bool f(const char *a) { return a != nullptr; }
static_assert([v = g{}] { return f(&v.a); }());
