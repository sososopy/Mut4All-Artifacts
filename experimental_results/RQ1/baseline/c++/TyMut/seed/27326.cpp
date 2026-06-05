
#include <concepts>

template <typename T>
void f(T t) {
  auto g = []<typename U>(U u) {
    if constexpr (requires { std::integral<U>; });
  };
  g(t);
}

int main()
{
  f(1);
}


