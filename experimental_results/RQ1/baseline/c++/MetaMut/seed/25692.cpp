
#include <initializer_list>

int foo(std::initializer_list<int&&>);

int i = foo({ 0 });
