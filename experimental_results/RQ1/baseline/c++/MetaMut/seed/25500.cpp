
template <typename T1, typename T2, typename... Types>
void foo(T2, Types...)
{
}

template <typename T1, typename... Types>
void foo(int, Types...)
{
}

int main()
{
  foo<void>(1, 2);
}

