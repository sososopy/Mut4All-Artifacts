template<typename T>
concept foo = true;

template<typename T>
void bar(T t)
{
  if constexpr (foo<T>::value)
  {
  }
}

int main()
{
  bar(1);
}

