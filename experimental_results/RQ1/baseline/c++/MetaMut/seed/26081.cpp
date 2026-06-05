
namespace std
{
  template<typename T, typename U>
    void remove(T, U)
    { }
}

int remove(char);

namespace std
{
  using ::remove;
}

namespace std
{
  template void remove(int, long);
}

