
template <int a> struct f{};

template<double setup> struct knob_t {
  int value = setup;
};
struct Helpers
{
  knob_t<1.0> inputs;
};
template <class T, int N>  auto h(f<N> a) noexcept -> decltype(sizeof(T{}));
int t = h<Helpers>(f<1>{});
