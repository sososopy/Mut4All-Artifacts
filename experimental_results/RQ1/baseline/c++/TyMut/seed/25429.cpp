
struct __attribute__((abi_tag("A", "B"))) A { };
struct A18 {
  operator A() { return A(); }
};
void f18_test() {
  A a = A18();
}

