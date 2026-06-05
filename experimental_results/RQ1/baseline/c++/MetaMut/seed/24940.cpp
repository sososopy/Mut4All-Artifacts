
typedef int v4si __attribute__ ((vector_size (16)));

v4si
fn1 (int i)
{
  v4si r;
  r = i <= (v4si){(0, 0)};
  return r;
}

v4si
fn2 (int i)
{
  struct S { v4si v; };
  struct S s = { .v = i <= (v4si){(0, 0)} };
  return s.v;
}
