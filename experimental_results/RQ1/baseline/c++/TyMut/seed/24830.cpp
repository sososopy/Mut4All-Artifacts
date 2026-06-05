
char foo;

constexpr unsigned char bar(__UINTPTR_TYPE__ baz)
{
  switch (baz)
    {
    case 13:
      return 11;
    case 14:
      return 78;
    case 2048:
      return 13;
    default:
      return 42;
    }
}

__attribute__ ((__noipa__))
void xyzzy(int x)
{
  if (x != 42)
    __builtin_abort ();
}

int main()
{
  unsigned const char c = bar(reinterpret_cast<__UINTPTR_TYPE__>(&foo));
  xyzzy(c);
}
