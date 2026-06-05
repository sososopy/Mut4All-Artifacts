
struct dummy0 { int i; };
struct dummy { struct dummy0 d[0]; };
extern void fun(struct dummy, int);
void
foo (void)
{
  struct dummy d;
  fun(d, 1);
}
