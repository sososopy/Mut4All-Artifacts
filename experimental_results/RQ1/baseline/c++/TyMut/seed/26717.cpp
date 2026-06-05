
struct source_location {
  const int ln;
  constexpr source_location(int ln = __builtin_LINE ()) noexcept: ln (ln) { }
  constexpr int line () const noexcept { return ln; }
  static constexpr source_location current () noexcept {
      return source_location (__builtin_LINE ());
  }
};

#define LINE 123
#line LINE
constexpr source_location loc = source_location::current ();
static_assert (loc.line () == LINE, "source_location::line");

