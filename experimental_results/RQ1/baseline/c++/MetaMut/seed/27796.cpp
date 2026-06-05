
typedef char a;
using b = int;
class c {
  b f{5};
};
class d {
  int e{};
};
class C {
  C() = delete;
};
class i : public d, public C {};
template <int, typename, int, class> using g = i;
enum { h };
using code = int;
struct : c {
  enum k : a;
  g<h, k, h, code> j;
} ags{};

