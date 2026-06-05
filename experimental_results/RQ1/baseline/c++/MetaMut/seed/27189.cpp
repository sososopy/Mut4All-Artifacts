#include <utility>

auto operator|(auto S, auto) {
  return S;
}

int main()
{
  decltype(std::index_sequence<0>{} | 0 | 0);
}

