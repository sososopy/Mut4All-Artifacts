template <typename T>
  class A : T{};

template <typename T>
  using sp = A<T>;

struct Base {};

template <typename T, int num = 1>
const  sp<T>
  rec() 
{
  return rec<T, num - 1>();  
}

static void f(void) {
  rec<Base>();
}
