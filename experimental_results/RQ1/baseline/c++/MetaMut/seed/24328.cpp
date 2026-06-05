
#include <iostream>
#include <set>

int main()
{
  std::set<int> empty;
  std::set<int>::iterator iter = empty.begin() ;
  while (iter++ != empty.end())
  {
        std::cout << *iter << "\n";
  }
  return 0;
}
