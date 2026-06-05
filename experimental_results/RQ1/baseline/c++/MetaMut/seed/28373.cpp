
struct Q {
  int n;
  constexpr const Q* operator()(auto) const { return this; }
};

extern const Q q;

template<int>
constexpr const Q* p = q(0);
