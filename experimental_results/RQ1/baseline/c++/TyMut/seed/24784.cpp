
template <typename T>
void
foo (T x)
{
  #pragma GCC novector
  while (T y = x)
    {
      ++y;
    }
}

void
bar ()
{
  foo (-42);
}
