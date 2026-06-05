
struct dummy { };
struct foo
{
  int i1;
  int i2;
  int i3;
  int i4;
  int i5;
};

extern "C" void fun(struct dummy, struct foo);

int main()
{
  struct dummy d;
  struct foo f = { -1, -2, -3, -4, -5 };

  fun(d, f);
  return 0;
}


void fun(struct dummy d, struct foo f)
{
  if (f.i1 != -1)
    __builtin_abort();
}
