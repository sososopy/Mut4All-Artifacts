
#include <vector>

class A {
  int s;
public:
  A () = default;
  A (const A&) = delete;
  A& operator=(const A&) = delete;
  A (A&&) = delete;
  A& operator=(A&&) = delete;
};

int main() {
  std::vector<A> v;
  v.push_back (A {} );
}
