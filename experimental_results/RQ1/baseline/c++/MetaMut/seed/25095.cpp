
typedef int Trans_NS___cxx11_string;
class A {
public:
  template <class DerivedT> A(char *, DerivedT);
  template <class Fn, class A1, class A2>
  void m_fn1(char *, Fn, A1 const &, A2);
};
struct Dict {
  void m_fn2();
};
void fn1() {
  A a("", "");
  typedef void *Get;
  typedef void (Dict::*d)(Trans_NS___cxx11_string);
  a.m_fn1("", Get(), d(&Dict::m_fn2), "");
}

