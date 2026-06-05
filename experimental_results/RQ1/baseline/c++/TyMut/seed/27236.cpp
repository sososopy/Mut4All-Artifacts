

template <int a> struct b { static constexpr int c = a; };
template <typename, typename> struct g : b<false> {};
template <typename d> struct g<d, d> : b<true> {};
template <int, typename e, typename> struct j { typedef e af; };
template <typename e, typename k> struct j<false, e, k> { typedef k af; };
template <bool ag, typename e, typename k> using ah = typename j<ag, e, k>::af;
template <typename d, typename aj> constexpr bool ak = g<d, aj>::c;
template <typename> struct l;
template <long, typename d> using ao = typename l<d>::af;
template <unsigned long> struct m;
struct n {
  typedef m<0> ar;
};
template <typename d, d> struct o {};
template <typename, typename = n::ar> struct p;
template <typename d, unsigned long... av> struct p<d, m<av...>> {
  typedef o<d, av...> ar;
};
template <typename d, d> using aw = typename p<d>::ar;
template <long... av> using ax = o<unsigned long, av...>;
template <long at> using ay = aw<unsigned long, at>;
template <typename az> struct ba {
  template <typename bb> constexpr ba(bb q) : bd(q) {}
  static constexpr az be(ba &q) { return q.bd; }
  az bd;
};
template <unsigned long, typename...> struct bg;
template <unsigned long av, typename az> struct bg<av, az> : ba<az> {
  template <typename bb> constexpr bg(bb q) : ba<az>(q) {}
};
template <typename... bo> class r : public bg<0, bo...> {
public:
  template <typename... bm> constexpr r(bm... q) : bg<0, bo...>(q...) {}
};
template <typename az, typename... br> struct l<r<az, br...>> {
  typedef az af;
};
template <unsigned long bs, typename az> constexpr az bt(bg<bs, az> &q) {
  return bg<bs, az>::be(q);
}
template <int bs, typename... bo> constexpr ao<bs, r<bo...>> bu(r<bo...> q) {
  return bt(q);
}
template <typename... bo> constexpr r<bo...> bv(bo... q) {
  return r<bo...>(q...);
}
template <typename... bx> struct by {
  template <typename bz> static constexpr bool ca = (... || ak<bx, bz>);
};
class s;
template <typename cd> class t {
public:
  using bx = by<int>;
  constexpr void cf() {
    auto callbacks = cg().template ch<s>([](auto &ci) { return ci.cf(6); });
    cj(callbacks, 7);
  }
  constexpr void ck(int q) { cl = q; }
  int cl;
  template <typename... bz, unsigned long... h>
  constexpr void cj(r<bz...> q, int p2, ax<h...>) {
    (bu<h>(q)(p2), ...);
  }
  template <typename... bz> constexpr void cj(r<bz...> q, int p2) {
    cj(q, p2, ay<sizeof...(bz)>{});
  }
  constexpr auto &cg() { return *static_cast<cd *>(this); }
};
template <template <typename> typename...> class Q;
template <typename> class u {
public:
  using bx = by<s>;
  template <typename bz> constexpr auto cf(bz) {
    return [this](auto) {
      auto &ci = cg();
      ci.ck(1);
    };
  }
  int i;
  constexpr auto &cg() { return *static_cast<Q<t, u> *>(this); }
};
template <template <typename> typename... cp> class Q : public cp<Q<cp...>>... {
  struct L {
    template <typename cr> static typename cr::bx cs(int);
    template <typename cr> using by = decltype(cs<cr>(0));
  };
  template <typename...> struct v {
    template <typename cu> using cv = v<cu>;
    template <typename> using cw = v;
  };
  template <typename bz, typename... cx> struct v<bz, cx...> {
    template <typename cr>
    using cw =
        ah<L::template by<bz>::template ca<cr>, typename v<>::template cv<bz>,
           typename v<cx...>::template cw<cr>>;
  };
  template <typename> struct w;
  template <typename... cz> struct w<v<cz...>> {
    static constexpr auto &da(Q &q) { return (q.bu<cz>(), ...); }
    template <typename dc> static constexpr auto dd(Q &q, dc p2) {
      return bv(p2(q.bu<cz>())...);
    }
  };
public:
  template <typename cr> constexpr auto &bu() {
    return *static_cast<cr *>(this);
  }
  template <typename> constexpr auto &de() {
    return w<typename v<cp<Q>...>::template cw<int>>::da(*this);
  }
  template <typename cw, typename dc> constexpr auto ch(dc q) {
    return w<typename v<cp<Q>...>::template cw<cw>>::dd(*this, q);
  }
};
void f() {
  auto f = [] {
    Q<t, u> derived{};
    auto &df = derived.de<int>();
    df.cf();
    return df;
  };
  constexpr auto dg = f();
}
