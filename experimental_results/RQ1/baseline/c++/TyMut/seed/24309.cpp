
#include <memory>
#include <map>

int
main()
{
  std::unique_ptr<int> ptr{new int};
  std::map<int, std::unique_ptr<int>> map;
  map.insert({0, std::move(ptr)});
  return 0;
}
