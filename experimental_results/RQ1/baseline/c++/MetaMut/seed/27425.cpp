
#include <utility>
using namespace std;

struct foo {
  int a,b,c;
  // expect-warning
  foo(const foo &);
};

foo fn(foo &&f) {
  foo g(move(f));
  return g;
}
