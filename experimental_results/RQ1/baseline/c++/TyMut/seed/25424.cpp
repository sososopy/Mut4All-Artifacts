
int fn1() {
  struct S *x;
  __asm ( "": :"" (*x));
}
