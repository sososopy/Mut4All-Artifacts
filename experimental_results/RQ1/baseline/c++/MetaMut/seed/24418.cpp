
  #include <stdio.h>

  void* test(void) {
    return printf("Hello world.\n"), nullptr;
  }

  int main(void) {
    return test() ? 0 : 1;
  }

