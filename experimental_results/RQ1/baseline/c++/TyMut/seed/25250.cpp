
struct S {
  int n; 
  char *a[];
};

void foo (const char *a)
{
  const S s = { 1, { a, "b" } };
}

