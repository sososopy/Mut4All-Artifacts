

template <typename T, typename... Args>
void resolver(int (*) (T, Args...));

int funcA(int, float) { return 0; }
int funcA(double) { return 0; }

int
main (void)
{
  resolver<int> (&funcA);
}
