

template <typename T1, typename T2> class tuple {
public:
    template <typename = T2>
    constexpr tuple() {}
};
template <class X, class... Y> struct a : public X {
  using X::X;
};
auto x = a<tuple<int, int> >();
