
#include <iostream>

template <class... Ts> struct X;
template <class T> struct X<T> { };

template <class... Us> struct Y { X<Us...> x; };

template <class... Ts>
std::ostream& operator<<(std::ostream& os, Y<Ts...> const& ) {
  os << std::endl;
  return os;
}

int main()
{ }

