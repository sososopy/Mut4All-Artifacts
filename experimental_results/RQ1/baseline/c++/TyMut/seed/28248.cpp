
struct S { ~S(); };
int
foo ()
{
  S s;
  if constexpr (true)
    return 0;
  else
    return 1;
}

#if __cpp_if_consteval >= 202106L
int
bar ()
{
  S s;
  if consteval
    {
      return 0;
    }
  else
    {
      return 1;
    }
}
#endif
