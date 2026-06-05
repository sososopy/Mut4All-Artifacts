
template <typename = int> class A {
  const int &m_fn1() { return 0; }

public:
  template <typename _Kt> void m_fn2(_Kt) { m_fn1(); }
};
class B {
  A<> _M_t;

public:
  template <typename _Kt> auto m_fn3(_Kt p1) -> decltype(_M_t.m_fn2(p1)) {
    _M_t.m_fn2(p1);
  }
};
struct C {};
void operator<(C, int) {
  B a;
  a.m_fn3(C{});
}
