
int bar();

void foo()
{
  const int t = bar();
  auto f = [=](auto x){ return t; };
  f(0);
}

