
template <typename T = int>
int fn()
{
  return 1;
}

template <typename T>
void bar() {
  const int i = int{fn()};
}
