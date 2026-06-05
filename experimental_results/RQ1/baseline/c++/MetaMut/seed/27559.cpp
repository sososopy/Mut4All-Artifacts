
#include <memory>
#include <unordered_map>
class A {
 public:
  A (int a) { i = a; }
  int i;
};
int main (void) {
  std::unordered_map<int, std::unique_ptr<A>> m;
  m.emplace (1, new A(1));
  return 0;
}
