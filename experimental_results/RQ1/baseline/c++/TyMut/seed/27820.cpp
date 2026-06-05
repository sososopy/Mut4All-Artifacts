
struct foo { int a; };
int main()
{
  [](auto) {
    if (int x = []{ return 0; }()) {
      foo a;
      (void) a;
    }
  }(0);
  return 0;
}

