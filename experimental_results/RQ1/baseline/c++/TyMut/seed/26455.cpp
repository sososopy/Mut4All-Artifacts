
template <typename T>
T foo (T arg, T& ref, T* ptr)
{
  int a = 1;
  return static_cast<T!>(a);
}

