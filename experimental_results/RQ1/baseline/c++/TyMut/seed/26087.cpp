
class B {
 public:
  virtual int add (int a, int b) {return a+ b;}
};

class D : public B {
};

int foo (int a, int b) {
  D d;
  return d.add(a, b);
}

