
constexpr int
foo (int a, int b)
{
  if (__builtin_is_constant_evaluated ())
    return a + b;
  asm("nop");
  return a;
}
