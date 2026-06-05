
template <typename> struct a { a(int); };
template <typename> struct b : a<b<int>> { using a<b>::a; };
template <typename c> b(c)->b<c>;
struct e {
  template <typename d> e(d);
} e{b{2}};
