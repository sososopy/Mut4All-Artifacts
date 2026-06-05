
struct S {
  operator enum E {x, y} ();
};

int main (int argc, char* argv[]) {
  struct S s;
  return 0;
}

