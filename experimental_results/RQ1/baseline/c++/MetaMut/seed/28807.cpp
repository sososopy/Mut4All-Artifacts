
template <typename... Ts>
struct S {
  constexpr static auto x = Ts::x;
};

S s;

