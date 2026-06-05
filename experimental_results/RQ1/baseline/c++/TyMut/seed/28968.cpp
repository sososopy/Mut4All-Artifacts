
#include <sstream>
#include <iostream>

void foo(std::stringstream const&);

struct A {
    friend auto operator<<(std::ostream& os, A const&) -> std::ostream&;
};


int main() {
    foo(std::stringstream{} << A{});
    return 0;
}
