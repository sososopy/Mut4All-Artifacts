
namespace a {
  template <typename> struct b;
  template <typename c> using aa = c;
  template <typename> bool ab;
  struct ac;
  template <long, typename> using ad = ac;
  template <typename c> class ae { ae(c); };
}
namespace af {
  namespace ag { struct ah { typedef int ai; }; }
  template <typename, typename, typename, typename, typename = int, typename = int,
	    typename = int, typename = int, typename = int, typename = int, typename = int,
	    typename = int, typename = int, typename = int, typename = int, typename = int,
	    typename = int, typename = int, typename = int, typename = int, typename = int,
	    typename = int, typename = int, typename = int, typename = int, typename = int,
	    typename = int, typename = int, typename = int, typename = int, typename = int,
	    typename = int, typename = int, typename = int, typename = int, typename = int,
	    typename = int, typename = int, typename = int, typename = int, typename = int,
	    typename = int, typename = int, typename = int, typename = int, typename = int,
	    typename = int, typename = int, typename = int, typename = int>
  struct d { typedef ag::ah e; };
  namespace ag {
    template <template <typename> class aj>
    struct al { template <typename> struct am { typedef aj<int> e; }; };
    template <template <typename> class ak> struct an { typedef al<ak> ai; };
    template <template <typename> class ak> struct ao { typedef an<ak> e; };
    template <typename> struct aq;
    template <typename ak, typename ar, typename as, typename at, typename au, typename av,
	      typename aw, typename ax, typename ay, typename az, typename ba, typename bb,
	      typename bc, typename bd, typename be, typename bf, typename bg, typename bh,
	      typename bi, typename f, typename h, typename i, typename bj, typename j,
	      typename k, typename l, typename m, typename n, typename bk, typename bl,
	      typename bm, typename o, typename bn, typename p, typename q, typename r,
	      typename s, typename v, typename bo, typename br, typename bp, typename bq,
	      typename bu, typename bs, typename bt, typename bx, typename bv, typename bw,
	      typename ca, typename by>
    struct aq<d<ak, ar, as, at, au, av, aw, ax, ay, az, ba, bb, bc, bd, be, bf, bg, bh, bi,
	        f, h, i, bj, j, k, l, m, n, bk, bl, bm, o, bn, p, q, r, s, v, bo, br, bp, bq,
	        bu, bs, bt, bx, bv, bw, ca, by>> { typedef d<ak, bw, ca, by>::e e; };
    template <class bz, typename d> class F { static bool cb() { typedef typename bz::template am<typename d::ai>::e cc; new cc; } };
    template <template <typename> class, typename cd, typename d> class ce { static bool cb() { F<typename cd::ai, d>::cb; } };
  }
}
template <typename ap> concept cf = a::b<ap>::g;

struct {
  template <bool> struct ch;
  template <typename ci> using cj = ch<cf<ci>>;
  template <typename ci> auto operator()(ci &&) noexcept(cj<ci>()) {}
} begin;
template <typename ap> bool ck = requires(ap t) { begin(t); };
template <typename ap> bool cl = ck<ap>;
namespace a::cg {
  template <typename e> bool cn = cl<e>;
  template <typename e> bool cm = cn<e>;
}
template <typename t, typename> concept dc = requires(t co) { co; };
template <typename> concept cr = a::ab<>;
template <typename cp, cr cq> class K { template <dc<cp>> cq c(); };
namespace cs {
  template <typename ct> bool u = a::cg::cm<a::ad<0, ct>>;
  template <typename ct> bool cu = u<ct>;
}
template <typename w, typename x> requires cs::cu<a::aa<w>> void y(w, x);
namespace cv::alignment::cw::cx::affine::cy { auto z = [] {}; auto cz = [] {}; auto da = [] {}; auto db = [] {}; }
using namespace cv::alignment::cw;
template <auto> struct dg;
template <typename> class dd;
using de = af::d<dg<cx::affine::cy::z>, dg<cx::affine::cy::cz>, dg<cx::affine::cy::da>, dg<cx::affine::cy::db>>;
namespace df { template <typename> class H { virtual void dh(); }; }
template <typename di> void df::H<di>::dh() { auto cw = this; auto dj = int{}, dk = cw; y(dk, dj); }
namespace df { typedef af::ag::ao<H>::e dl; }
bool dm = af::ag::ce<dd, df::dl, af::ag::aq<de>::e>::cb;

