template < int b > struct c {
  static constexpr int d = b;
  typedef c e;
};
template < bool, typename a > using f = a;
template < bool g > using h = c< g >;
template < int g > using i = c< g >;
template < typename... > struct ab {};
template < typename... j > struct k { using e = ab< j... >; };
template < typename > struct ad;
template < typename j, typename... l > struct ad< ab< j, l... > > {
  using e = ab< l... >;
};
template < typename > struct ae;
template < typename... m > struct ae< ab< m... > > : i< sizeof...(m) > {};
template < typename af, int, int = ae< af >::d > struct ag;
template < typename... m > struct ag< ab< m... >, 0, 0 > { using e = ab<>; };
template < typename... m, int ah >
struct ag< ab< m... >, 0, ah >
    : k< typename ag< typename ad< ab< m... > >::e, ah - 1 >::e > {};
template < typename... m, int ai, int ah >
struct ag< ab< m... >, ai, ah > : ag< typename ad< ab< m... > >::e, ai - 1 > {};
template < typename, template < typename > class > struct aj;
template < typename... j, template < typename > class ak >
struct aj< ab< j... >, ak > {
  using e = ab< typename ak< j >::e... >;
};
template < typename, template < typename > class > struct am;
template < typename > struct an {};
template < typename a, a > using ap = an< a >;
template < long... aq > using ar = an< unsigned long, aq... >;
template < long ao > using as = ap< unsigned long, ao >;
class au {
public:
  using e = h< c< false >::d >;
};
template < typename, template < typename > class, typename... >
struct ax : au::e {};
struct az {
  using e = ar<>;
};
template < typename > struct bb;
template < typename... j > struct bb< ab< j... > > {
  using e = as< sizeof...(j) >;
};
template < typename, typename > struct bc;
template < typename... j, long... ba > struct bc< ab< j... >, ar< ba... > > {
  using e = az::e;
};
template < typename j, template < typename > class ak >
struct bb< am< j, ak > > : bc< k<>::e, bb< k<>::e >::e > {};
namespace be {
enum bf { bg };
class bk {
public:
  enum bl { bm };
};
struct bn {
  static const bk::bl bo = bk::bm;
};
template < typename bq > struct br { using e = bq; };
template < typename, bf, int, typename... > class bs;
template < typename bq, bf bt, int bu, typename... j, long... anchors >
class bs< bq, bt, bu, ab< j... >, ar< anchors... > > : bk {
public:
  static const bl bo = bq::bo;
  static const long bv = bu;
  using bw = int;
  using bx = ab< j... >;
  using by = typename ag< bx, bu >::e;
  using bz = typename ag< bx, bu >::e;
  using ca = as< bv >;
  using cb = typename br< bq >::e;
  using c = decltype(cb()(bw(), by(), bz(), ca()));
};
class cf;
template < typename > struct cg : c< false > {};
template < typename bq, bf bt, int bu, typename... j >
class bs< bq, bt, bu, ab< j... > >
    : public bs<
          bq, bt, bu, typename aj< ab< j... >, cg >::e,
          typename bb< am< typename ag< ab< j... >, bu >::e, cg > >::e > {};
} using be::bf;
using be::cf;
template < typename cj > class ck {
  template < typename cl > ck(cl) { c< cj::bo >(); }
  static ck n;
};
namespace be {
template < typename > class cm;
template < typename j > using cn = ax< j, cm >;
template < typename, typename co >
void r(co, f< cn< co >::d, void * > = nullptr);
} using be::r;
struct cq : be::bn {
  template < typename... cs, typename... ct >
  auto operator()(int cu, ab< cs... >, ab< ct... >, ar<>) {
    r< cs... >(cu);
  }
};
template <>
ck< be::bs< cq, bf::bg, 1, ab< int, cf > > >
    ck< be::bs< cq, bf::bg, 1, ab< int, cf > > >::n(0);
