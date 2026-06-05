
#include <iostream>
#include <memory>

int main () {
  int arr[]={1,2};
  std::unique_ptr<int[]> up(arr);
  std::cout << up[0];
  return 0;
}
