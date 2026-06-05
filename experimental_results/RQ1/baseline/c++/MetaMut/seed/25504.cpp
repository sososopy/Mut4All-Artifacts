
#include <set>
#include <string>

using std::set;
using std::string;

int& IsNullLiteralHelper(...);

template <size_t> struct EqHelper { };

void Fn() {
  EqHelper<sizeof IsNullLiteralHelper(set<int>{1})>        eq1;  // ok
  EqHelper<sizeof IsNullLiteralHelper(set<string>())>      eq2;  // ok
  EqHelper<sizeof IsNullLiteralHelper(set<string>{"foo"})> eq3;  // error
}

