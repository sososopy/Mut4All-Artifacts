
struct A;
template <int> struct B {
  void m_fn1() { this->A::assign; }
};

