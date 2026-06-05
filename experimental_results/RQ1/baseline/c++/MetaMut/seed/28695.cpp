
#include <initializer_list>

struct S { S(int); };
void f(std::initializer_list<S>);
int f(int);

using T = decltype(f({1}));
using T = int;
