
#include <initializer_list>

struct A {};

std::initializer_list<volatile A> x = {{}};
