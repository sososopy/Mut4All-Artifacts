

#include <utility>

struct Do_Call
{
  template<typename Func, typename ... T>
  Do_Call(const Func &f, T && ... t)
  {
    f(std::forward<T>(t)...);
  }
};

int main()
{
  const auto sum = [](int w, int x, int y, int z) {
    return w + x + y + z;
  };
  
  int i = 0;
  Do_Call{sum, ++i, ++i, ++i, ++i};
}

