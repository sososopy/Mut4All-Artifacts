
struct A { char a[2]; };

static constexpr A foo () { return A{1}; }

void bar ()
{
  A a = foo ();
  if (a.a[0] != 1)
    __builtin_abort(); 
}

void foobar ()
{
  A a = foo ();
  if (a.a[0] != 1)
    __builtin_abort(); 
}
bar ()
{
  struct A a;

  try
    {
      a = {};
      a.a[0] = 1;
      _1 = a.a[0];
      if (_1 != 1) goto <D.2378>; else goto <D.2379>;
      <D.2378>:
      __builtin_abort ();
      <D.2379>:
    }
  finally
    {
      a = {CLOBBER};
    }
}


foobar ()
{
  struct A a;

  try
    {
      a = {};
      _1 = a.a[0];
      if (_1 != 1) goto <D.2380>; else goto <D.2381>;
      <D.2380>:
      __builtin_abort ();
      <D.2381>:
    }
  finally
    {
      a = {CLOBBER};
    }
}
