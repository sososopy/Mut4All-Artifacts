template<typename T>
  struct A {
    template<typename U>
      void f() { }
  };

int main() {
  A<void>().f(0);
}
