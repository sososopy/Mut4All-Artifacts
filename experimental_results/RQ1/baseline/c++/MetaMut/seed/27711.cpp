
__attribute__((noipa)) void
quux (unsigned long x)
{
  static int cnt;
  unsigned long v = cnt++ ? 6 : 0;
  if (x != v)
    __builtin_abort ();
}

__attribute__((noipa)) void
foo (const char *x)
{
  static int cnt;
  if (__builtin_strcmp (x, cnt++ ? "baz" : "qux") != 0)
    __builtin_abort ();
}

__attribute__((noipa)) void
bar (char *b)
{
  long c, e;
  for (c = 7; c >= 0; --c)
    if (b[c] != ' ')
      {
	b[c + 1] = '\0';
	break;
      }
  e = c + 1;
  unsigned long g = e;
  quux (g);
  foo (g ? "baz" : "qux");
}

int
main ()
{
  char buf[9];
  __builtin_memcpy (buf, "        ", 9);
  bar (buf);
  __builtin_memcpy (buf, "abcdef  ", 9);
  bar (buf);
  return 0;
}
