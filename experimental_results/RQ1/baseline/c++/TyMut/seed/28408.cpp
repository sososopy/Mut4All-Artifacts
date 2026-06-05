
#include <unordered_set>
void f (std::unordered_set<void*>& s)
{
  s.insert (nullptr);
}
#include <map>
std::map< int, int> x;
