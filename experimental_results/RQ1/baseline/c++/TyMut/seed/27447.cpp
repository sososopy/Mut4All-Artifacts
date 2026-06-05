
template<typename R, typename... Types>
using Function = R(*)(Types...);

struct Struct
{
  void *addr;

  template<typename R, typename... Types>
  constexpr Struct(Function<R, Types...> addr) : addr((void*)(addr)) {}
};

void TestFunction()
{
}

int main(void)
{
  static constexpr Struct functions[]{
    {TestFunction},
  };
}

