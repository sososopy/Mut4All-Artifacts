
#include <iostream>
#include <concepts>

#define CHECK(e,T)							\
  std::cout << "Is " << #e << " of type " << #T << "? " << requires {	\
    { e } -> std::same_as<T>;						\
  } << std::endl

int
main()
{
  std::cout << std::boolalpha;

  int i = 0;
  CHECK(0, int);
  //CHECK(0, int&);
  CHECK(i, int);
  CHECK(i, int&);
}
