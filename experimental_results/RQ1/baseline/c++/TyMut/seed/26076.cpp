
struct A { bool a : 1; bool b : 1; };
struct B
{
  unsigned char c[0x40000000];
  unsigned char d[0x40000ff0];
  A e;
  void *foo ()
  {
    if (e.a)
      return (void *) 0;
    e.b = true;
    return c;
  }
};
void
bar (B *p)
{
  p->foo ();
}

