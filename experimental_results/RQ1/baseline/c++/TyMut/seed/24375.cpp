class A { };
typedef A B;
void foo (B &a) {
  a.x();
}

