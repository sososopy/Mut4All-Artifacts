
template<typename T> __attribute__((assume_aligned(sizeof(int(T()))))) T *f();
void test21() {
  void *p = f<void>()
}

