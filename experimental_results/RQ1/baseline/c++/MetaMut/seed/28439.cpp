
int
__attribute__((pure))
foo(int *x);

int main()
{
  int a;
  int r = foo (&a);

  if (a != 123)
    __builtin_abort ();
}
$ cat repro2.cc
int
__attribute__((pure))
foo(int *x)
{
  *x = 123;
  return 0;
}

