
#include <vector>

template <typename T>
void blub(std::vector<T> const& i) {
  -i;
  //getsize(i); //this also fails in gcc
}

int operator-(std::vector<int> const& i) {
    return i.size();
}

int getsize(std::vector<int> const& i) {
    return i.size();
}

int main() { blub(std::vector<int>{1}); }
