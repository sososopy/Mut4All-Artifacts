
template <int, int, bool, class> class a;
namespace b {
template <typename, unsigned long, long, unsigned long, typename...> class b;
template <unsigned long, typename> struct c;
template <typename l, unsigned long d, unsigned long dim, typename... curshape>
struct c<d, b<l, dim, 0, dim, curshape...>> {
  using e = typename c<d - 1, b<l, dim, 0, dim, a<0, 1, true, void>>>::e;
};
template <typename l, unsigned long dim, typename... curshape>
struct c<0, b<l, dim, 0, dim, curshape...>> {
  using e = b<l, dim, 0, dim, curshape...>;
};
template <typename l, unsigned long f,
          template <int, int, bool, typename> class baseclass, int g, int h,
          typename i, typename... j>
class b<l, f, 0, f, baseclass<g, h, true, i>, j...> {
  b(long);
  b(const long (&)[]);
};
} // namespace b
template <long dim, typename l = double>
using blaspp = typename b::c<dim, b::b<l, dim, 0, dim>>::e;
blaspp<1> k({4})

