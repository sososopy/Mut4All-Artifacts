struct B {
  B() = default;
private:
  B(const B&);
};

struct A {
  B b;

  template<class T>
  static void f() { new A(); }
};

int main() {
  A::f<int>();
}

