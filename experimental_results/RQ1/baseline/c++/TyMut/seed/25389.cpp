
struct A {
  A(const int i, const int j) {};
};

struct B1 : virtual public A {
  virtual void moo()=0;
  B1() {};   // (1) Look! not "B1() : A(5,6) {};"
};

struct B2 : virtual public A {
  virtual void cow()=0;
  B2() {};   // (2) Look! not "B2() : A(7,8) {};"
};

struct C : public B1, public B2 {
  C() : A(2,3) {};
  void moo() {};
  void cow() {};
};

int main() {
  C c;
  return 0;
};

