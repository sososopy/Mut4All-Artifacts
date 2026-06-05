
void bar()
{
  int foo();
  int foo() __attribute__((target("sse")));
  int foo() __attribute__((target("avx")));
  int (*p)() = &foo;
  return;
}

