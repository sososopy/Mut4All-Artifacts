
struct s
{
  struct {} a[] = 1.0;
  void f (char *c)
  {
    s s;
    __builtin_memcpy (&s, c, sizeof(s));
  }
};

