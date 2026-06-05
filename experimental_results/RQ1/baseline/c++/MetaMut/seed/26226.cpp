
struct Base {
  int x;
};

struct Child : Base {
  __int64 y;
  virtual void foo() {}
};

extern "C" int printf(const char *fmt, ...);

int main() {
  Child s;
  printf("x offset = %lu\n", (unsigned long)&s.x - (unsigned long)&s);
  printf("y offset = %lu\n", (unsigned long)&s.y - (unsigned long)&s);
}
