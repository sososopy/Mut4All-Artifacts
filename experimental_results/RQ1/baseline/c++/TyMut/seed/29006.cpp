
struct A {
  static void f();
};

template<class>
struct B : private A {
  struct C {
    void g() { f(); }
  };
};

int main() {
  B<int>::C().g();
}


