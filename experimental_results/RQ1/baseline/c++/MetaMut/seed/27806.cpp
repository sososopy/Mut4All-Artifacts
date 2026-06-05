
void f() {
  static int i = ({
    if (true) return;
    42;
  });
}

