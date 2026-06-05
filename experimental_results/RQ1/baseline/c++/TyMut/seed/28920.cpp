
#include <utility>

struct S {};

template <typename... Args>
void foo(Args&&... args) {
  [...args = std::forward<Args> /*(args)*/] {
    [](auto...) { } (std::forward<Args>(args)...);
  };
}

void bar( ) {
  foo(S{});
}

