

template <typename... T> struct S : T... {
  template <typename U, typename... V> S(U u, V... v) : T(u, v...)... {}
};
struct R {
  R(int);
};
void h() { S<R>(3); }
