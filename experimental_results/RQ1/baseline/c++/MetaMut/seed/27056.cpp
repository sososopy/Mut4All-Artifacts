
class A {
public:
  A() {}

  static const A& a() {
      static A _a = A();
      return (_a);
  }

  virtual A& operator= (const A&) {
      return *this;
  }
};

int main() {
  return 0;
}

