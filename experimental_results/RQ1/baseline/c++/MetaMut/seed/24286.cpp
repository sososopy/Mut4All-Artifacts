
template <unsigned N> struct require_constexpr {
  static constexpr unsigned value = N;
};
template <typename...> constexpr void noop (void) { }

// a constexpr constructible class
struct test { 
  constexpr unsigned size() const { return 0; }
  constexpr test() { }
  constexpr test(const test &) = default;
};

// size wrappers
constexpr auto size0 (test t) { return t.size(); }
// just making sure type "test" is still considered
constexpr auto size1 (test t) { return size0(t); }
// constexpr constructible outside a template argument
constexpr auto size2 (test t) { return size1(t); }
// PROBLEM here
constexpr auto size3 (test t) { return require_constexpr< size0(t) >::value; }

int main (int, char**)
{
  constexpr auto const ar = test { };

  noop<
    require_constexpr< size3(ar) >
  >();

  return 0;
}
