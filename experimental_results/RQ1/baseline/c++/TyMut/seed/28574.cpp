
thread_local int *z;

void f() {
  extern thread_local int z;
}
