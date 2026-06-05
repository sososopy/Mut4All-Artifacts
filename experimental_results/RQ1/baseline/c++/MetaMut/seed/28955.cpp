
struct a {
  constexpr a(const char *) : b{}, c{} {}
  long b;
  char c;
};
using d = a;
struct e;
struct f;
template <int> struct m { typedef e g; };
template <int h> struct o {
  typename m<h>::g i;
  void operator[](long) const;
};
struct e {
  d j;
  void (*k)(f *);
};
struct f {
  int l;
  struct p {
    static constexpr o<1> fields{"", [](f *n) { n->*&f::l = 2; }};
  };
};
int main() { f::p::fields[0]; }

