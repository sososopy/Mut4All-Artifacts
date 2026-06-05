
class shared_count {
public:
  ~shared_count() { delete this; }
};
class A {
  shared_count pn;
};
class B {
public:
  B(bool);
  A m_message;
};
void fn1(B) { fn1(0); }

