
#pragma GCC visibility push(hidden)
namespace std { class type_info; }
#pragma GCC visibility pop
#include <typeinfo>
bool foo(std::type_info& a, std::type_info& b) { return a == b; }
