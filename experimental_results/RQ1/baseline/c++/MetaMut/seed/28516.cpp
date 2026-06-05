
struct Q {
  struct P {
    const Q* p;
  };
  int n;
  constexpr P operator()(auto) const { return {this}; }
};

extern const Q q;

#if 1
template<int> constexpr auto p = q(0);
static_assert(p<0>.p == &q);
#else
constexpr auto p = q(0);
static_assert(p.p == &q);
#endif

constexpr Q q = {};
