
#include <string>
#include <map>
#include <functional>
#include <utility>
#include <future>

template <typename T>
std::future<T> make_ready_future(T t);

struct y
{
  int                                v;
  std::function<void()> v2 = [this]()  { v; };
};

template <typename... Ts>
auto xx(Ts... ts)
{
  return make_ready_future(y{ ts... });
}

int main() {
    auto f = xx();
}
