
#include <string>
#include <tuple>

std::tuple<int&&>               A{1};   // ok
std::string&&                   B{""};  // ok
std::tuple<std::string&&>       C{""};  // ICE
//int  main() {};
