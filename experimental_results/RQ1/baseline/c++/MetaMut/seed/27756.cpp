namespace a {
  struct b;
  template <typename...> using c = b;
}
template <typename... d> struct e : a::c<d...> { using a::c<>::c; };
template <template <typename> typename f> void g() { f(); }
void h() { g<e>(); }

