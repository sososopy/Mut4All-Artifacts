
template <typename a> struct b { a c; };
class d {
  d(d &);
};
struct {
  const b<d> *operator->();
} e;
d f() {
  auto g = e;
  return 0 ? g->c : throw
}
