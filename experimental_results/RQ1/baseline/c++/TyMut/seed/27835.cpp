
namespace a {
template <int c> struct d { static constexpr int f = c; };
template <typename> struct g;
template <typename b, typename h = b> h i(int);
template <typename b> auto ab() -> decltype(i<b>(0));
} // namespace a
namespace aa {
template <bool ad> using ae = a::d<ad>;
struct af {
  template <typename ac> using ag = ac;
};
template <bool> using ah = af;
template <bool ai, typename ac> using aj = typename ah<ai>::template ag<ac>;
} // namespace aa
namespace a {
template <typename b> class ak {
public:
  ak(b);
};
} // namespace a
namespace al {
template <typename> struct am;
template <typename> using an = am<int>;
template <int> struct v { template <typename, typename ao> using ap = ao; };
template <bool ad, typename ac, typename ao>
using aq = typename v<ad>::template ap<ac, ao>;
struct as {
  template <typename> void at();
};

bool e();
namespace ar {
template <typename> concept bool au = requires { as::at; };
template <typename, typename> concept bool j = requires { e(); };
template <typename, int> struct k;
template <typename av> struct k<av, true> : aa::ae<j<av, as>> {};
template <typename av> struct l : k<av, au<av>> {};
template <typename av, int> struct m { using aw = aq<l<const int>::f, const int, av>; };
template <typename av> using n = m<av, au<av>>;
} // namespace ar
template <typename av> struct am : ar::n<av> {
  using ax = ar::n<av>;
  using typename ax::aw;
  auto operator*() -> aw;
  auto operator++() -> am;
};
template <typename av, typename ay> auto operator!=(av, ay) -> bool;
template <typename ac> concept bool az = a::g<ac>::f;
struct {
  template <int> struct o {};
  template <typename ba> using bb = o<az<ba>>;
  template <typename ba> auto operator()(ba) noexcept(noexcept(bb<ba>{})) {}
} end;
template <typename u> using p = decltype(a::ab<u>);
template <typename ac> concept bool w = requires(ac q) { end(q); };
template <typename ac> concept bool r = w<ac>;
template <typename ac> concept bool bc = r<ac>;
struct x {
  template <typename bd, typename be>
  friend auto operator|(bd, be) -> decltype(be ::bf(a::ab<bd>, a::ab<be>()));
};
namespace bg {
template <typename bh> struct y : x {
  bh s;
  template <typename u, typename bi> static auto bf(u z, bi bj) {
    return bj.s(z);
  }
};
} // namespace bg
struct bk {
  template <bool bl = true> auto begin() -> an<p<aa::aj<bl, int>>>;
  int end();
};
namespace bg {
struct bm {
  template <typename u> auto operator()(u) -> bk requires bc<u>;
};
y<bm> bn;
} // namespace bg
} // namespace al
void b() {
  a::ak t{4};
  for (auto e : t | al::bg::bn)
    ;
}
