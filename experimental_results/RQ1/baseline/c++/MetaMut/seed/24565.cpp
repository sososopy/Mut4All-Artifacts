
#include <utility>

template<typename T>
auto greater_than(T&& t)
{
  return [val = std::forward<T&&>(t)] (const auto& arg) { return arg > val; };
}



template<typename Functor>
void g(Functor f)
{
  for (int i = 0; i < 100000; i++)
    f(i);
}

int main()
{
  g(greater_than(10));
}

