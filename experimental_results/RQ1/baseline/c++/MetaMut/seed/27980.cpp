
// start of source code
#include <string_view>
#include <initializer_list>

struct S { const int i; };

constexpr std::initializer_list<S> foo{
    { []() { return std::string_view("").compare(""); }() }
};
// end of source code
