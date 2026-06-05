

#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
using namespace std;

int main() {
  vector<pair<string, int>> vec2 = {
    pair<string, int>("aaa", 3),
    pair<string, int>("baaa", 4),
    pair<string, int>("bbaaa", 5),
    pair<string, int>("bbaac", 5),
    pair<string, int>("bbbaaa", 6),
    pair<string, int>("cccddd", 6),
    pair<string, int>("ddd", 3)};

  sort(vec2.begin(), vec2.end(),
       [](const pair<string, int> &p1, const pair<string, int> &p2) {
         cout << p1.second << " vs " << p2.second << endl;
         // No problem with ">"
         // if (p1.second > p2.second) { return true; }

         // Segmentation fault: 11 with ">=",
         // it's related to the data in vec2.
         if (p1.second >= p2.second) { return true; }

         return false;
       });

  for (const auto &p : vec2) {
    cout << p.first << ", " << p.second << endl;
  }
  cout << endl;
}

