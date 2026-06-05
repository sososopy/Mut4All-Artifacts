

inline void foo(int* ptr) {
  100 / ptr[0];
}

void bar(int* ptr) {
  *ptr = 40;
  foo(ptr):
  *ptr = 50;
}

=>

inline void foo(int* ptr) {
  // 100 / ptr[0]; removed, dead code
}

void bar(int* ptr) {
  *ptr = 40;
  foo(ptr):
  *ptr = 50;
}

=>

inline void foo(int* ptr) {
  // 100 / ptr[0]; removed, dead code
}

void bar(int* ptr) {
  // *ptr = 40; dead store, since foo does not read memory
  foo(ptr):
  *ptr = 50;
}

