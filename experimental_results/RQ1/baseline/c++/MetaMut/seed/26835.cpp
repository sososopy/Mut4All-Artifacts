
namespace std {
template <class> class initializer_list {
  int *_M_array;
  int _M_len;
};
class A {
public:
  void operator=(initializer_list<int>);
};
class B {
  void m_fn1(A &) const;
};
void B::m_fn1(A &) const {
  A extra;
  extra = {};
}
}

