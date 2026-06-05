

struct A;
template <typename> class C;
using PathComponentPiece = C<int>;
class B {
  B(int);
  template <typename T> B(T);
  B(C<A>);
};
template <typename> class C : B {
  using base_type = B;
  base_type::base_type;
  PathComponentPiece m_fn1() {}
};

