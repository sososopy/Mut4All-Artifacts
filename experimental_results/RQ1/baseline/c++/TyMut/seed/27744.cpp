
extern "C" __inline __attribute__((__gnu_inline__)) int snprintf(...) {}
class a {
  char b[4096];
  void c();
};
void a::c() {
  char d[4096];
  snprintf(d, sizeof(d), "%s/power/runtime_suspended_time", b);
}
