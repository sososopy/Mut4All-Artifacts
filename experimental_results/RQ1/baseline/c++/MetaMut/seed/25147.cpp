
struct A {
  struct struct2 aligned;
};
void fn1(int) {
  A a;
  fn1((int &)a);
}
