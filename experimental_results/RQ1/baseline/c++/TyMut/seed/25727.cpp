#include <iostream>
#include <array>
#include <vector>

using namespace std;

int main() {
  std::vector<int> hello;
  //hello.resize(3);  //Fixes the problem
  hello[0] = 1;
  hello[1] = 2;
  hello[2] = 3;

  for (auto i = hello.begin(); i != hello.end(); ++i)
    printf("%d\n", *i);

  return 0;
}
