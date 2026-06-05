
int f() {
  thread_local int n;
  auto l = [] { return n; };
  return l();
}

