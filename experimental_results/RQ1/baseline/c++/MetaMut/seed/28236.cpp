

int f(int x) [[pre: x > 0]] [[post r: r > 0]]
{
  return -5;
}

int main()
{
  f(42);
}
