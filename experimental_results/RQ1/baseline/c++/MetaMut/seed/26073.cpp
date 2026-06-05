
template <class T>
struct S {
  typedef T T_;
  typename typedef T_::D D;
};

struct U {
  typedef int D;
};

S<U> x;
