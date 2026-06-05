
class A;

class C {
  void m_fn1();
  A *a_;
};

class A {
public:
  void m_fn2();
  int elements_[8];
  int num_elements_;
};

int a;
void A::m_fn2() {
  int b = 0;
  if (num_elements_)
    ++b;
  for (int i = b + 1; i < num_elements_; ++i) {
    if (elements_[i])
      ++a;
  }
}

void C::m_fn1() {
  a_->m_fn2();
}
