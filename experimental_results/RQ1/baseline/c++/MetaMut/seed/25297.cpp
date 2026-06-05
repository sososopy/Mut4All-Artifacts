

constexpr int
foo (int p)
{
  int t = 0; 
  while (1) 
    ;
  return t;
}

static_assert (foo (1) == 0, "");
