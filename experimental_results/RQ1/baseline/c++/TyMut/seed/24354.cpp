
struct ListSyntax {
  ListSyntax(int, bool);
};
struct A {
  template <typename... Args>
  ListSyntax *m_fn1(Args &... args) {
    return new ListSyntax{args...};
  }
};
struct B {
  const ListSyntax *m_fn2(const int &, bool);
  A lists;
};
const ListSyntax *B::m_fn2(const int &elts, bool dot) {
  return lists.m_fn1(elts, dot);
}

