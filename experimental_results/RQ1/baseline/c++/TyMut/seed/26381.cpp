
struct f{};
int &&ffa(const f&, int &r) { return static_cast<int&&>(r); }
void b() {
  int a;
  int&& i = ffa(f{}, a);
  i = 1;
}
