
struct X {
  const X* x = this;
};
constexpr const X& x = X{};
