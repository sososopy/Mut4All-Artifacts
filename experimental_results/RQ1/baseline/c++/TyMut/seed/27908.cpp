

#include <string>
#include <iostream>
#include <variant>

using namespace std;

template<class... Ts> struct Visitor : Ts... {
    using Ts::operator()...;
};
template<class... Ts> Visitor(Ts...) -> Visitor<Ts...>;

int main()
{
    std::variant<int, string> package;

    auto x = Visitor {
            [](int)    { cout << "int\n"; },
            [](string) { cout << "string\n"; },
            }
    ;
    std::visit(x, package);
}




