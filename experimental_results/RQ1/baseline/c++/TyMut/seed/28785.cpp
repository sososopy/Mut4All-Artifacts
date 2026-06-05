
#include <vector>
#include <algorithm>

int main(){
  std::vector<int> a(2,1);
  std::vector<int> b(2,1);
  std::lexicographical_compare_three_way(a.begin(),a.end(),b.rbegin(),b.rend());
  return 0;
}

