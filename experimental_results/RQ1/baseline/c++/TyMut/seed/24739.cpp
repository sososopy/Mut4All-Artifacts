
#include <stdio.h>

void foo(const char *) {}

template<class T> decltype(foo(T())) red(T) { puts("yes"); }
void red(...) { puts("no"); }

int main() {
    foo(int());  // should not compile
    red(1);  // should print "no", not "yes"
}
