#include <iostream>
#include <vector>

using namespace std;

int main() {
  std::vector<int> hello;
  //hello.resize(1);  //Fixes the problem
  hello[0] = 1;

  return 0;
}
