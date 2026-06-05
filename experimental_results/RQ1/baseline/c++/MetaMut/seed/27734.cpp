#include <optional>
#include <string>

int e;

template <typename T> std::optional <size_t> f ()
{
  if (auto i = std::string ().find ('a'))
    return i;
  return { };
}

int main ()
{
  size_t j = 1;
  while (j)
    {
      auto i = f <int> ();
      if (e != 1 && e != 4 && e != 7)
        throw 0;
      if (i)
        j = *i;
    }
}
