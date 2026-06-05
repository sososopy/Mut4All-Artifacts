
class TheClass {};
void the_func() {
  TheClass x;
  volatile TheClass y;
  (false ? x : x) = y;
}

