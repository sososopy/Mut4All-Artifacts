

int* f(void) {
  static int* p = new int;
  return p;
}

