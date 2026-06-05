
extern "C" int printf(const char *fmt, ...);

struct S {
  int a, b, c;
};

struct This {
  S __cdecl foo(S x);
  int field;
};

int main(void) {
  This obj;
  obj.field = 13;
  S arg;
  arg.a = 99;
  S s = obj.foo(arg);
  printf("obj.field = %d\n", obj.field);
  return 0;
}
